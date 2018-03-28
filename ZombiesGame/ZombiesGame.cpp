#include "ZombiesGame.h"
#include <GameEngineOpenGL\GameEngineOpenGL.h>
#include <GameEngineOpenGL\ResourceManager.h>
#include <GameEngineOpenGL\ErrManager.h>
#include <glm\gtx\rotate_vector.hpp>

#include <time.h>
#include <random>
#include <iostream>
#include <algorithm>

#include "Gun.h"
#include "Zombie.h"

const float HUMAN_SPEED = 2.5f;
const float ZOMBIE_SPEED = HUMAN_SPEED * 1.2f;
const float PLAYER_SPEED =  HUMAN_SPEED * 2.f;
const float BULLET_SPEED = 10.f;
const float DESIRED_FPS = 60.f;
const int MAX_TIME_STEPS = 6;
const float MAX_DELTA_TIME = 1;
const float MILLISEC_PER_SEC = 1000.f;
const float CAMERA_SCALE = 1.f / 2.5f;

ZombiesGame::ZombiesGame() :
	gameState_(GameState::PLAY),
	scrW_(1366),
	scrH_(768),
	maxFps_(60.f),
	currFps_(0),
	player_(nullptr),
	humansKilled_(0),
	zombiesKilled_(0)
{ /* empty */ }

// destructor
ZombiesGame::~ZombiesGame() { 

	for (size_t i = 0; i < levels_.size(); i++) {
		delete levels_[i];
	}

	// destroy humans
	for (size_t i = 0; i < humans_.size(); i++) {
		delete humans_[i];
	} 

	for (size_t i = 0; i < zombies_.size(); i++) {
		delete zombies_[i];
	}

	delete spriteFont_;
}

void ZombiesGame::run()
{
	initSystems();
	initGameProps();

	auto music = audioEngine_.loadMusic("Sounds/Suspense Loop.wav");
	music.play(-1);

	gameLoop();
}

void ZombiesGame::initSystems()
{
	// calling SDL_Init(SDL_INIT_EVERYTHING);
	ge::init();

	// initializing sound, must happen after ge::init()
	audioEngine_.init();

	// set up the camera
	camera_.init(scrW_, scrH_);
	hudCamera_.init(scrW_, scrH_);
	hudCamera_.setPosition(glm::vec2(scrW_ / 2, scrH_ / 2));

	// create the window
	window_.create("Zombies Game", scrW_, scrH_, ge::WINDOW_SHOWN);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // light gray background

	// Calling program to compile the shaders
	initShaders();
	agentSpriteBatch_.init();
	hudSpriteBatch_.init();

	// initializing sprite font 
	// ( must be initialized after initializing SDL, OpenGL and shaders )
	spriteFont_ = new ge::SpriteFont("Fonts/chintzy.ttf", 31);

	// initializing particles
	bloodParticleBatch_ = new ge::ParticleBatch2D();
	bloodParticleBatch_->init( 1000, 0.05f, 
		ge::ResourceManager::getTexture("Textures/particle.png"),
		[] (ge::Particle2D& p, float deltaTime)  
			{ p.pos += p.velocity * deltaTime; 
			  p.color.a = (GLubyte)(p.life * 255.f); }); // lampda for blood particles custom behavior

	particleEngine_.addParticleBatch(bloodParticleBatch_);

	// setting the FPS limiter
	fpsLimiter_.setTargetFps(DESIRED_FPS);
}

void ZombiesGame::initGameProps()
{
	// initializing level 1
	levels_.push_back(new Level("Levels/level_1.txt"));
	currLvl_ = 0;

	// initializing player
	player_ = new Player(
		levels_[currLvl_]->getPlayerStartPos(), 
		glm::vec2(0.f, 1.f), PLAYER_SPEED, 3, 
		&inputMngr_, &camera_, &bullets_);
	
	// Add player to vecor of humans
	humans_.push_back(player_);

	// random POS to use when initializing humans
	std::mt19937 randEngine;
	randEngine.seed(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution <int> randX(1, levels_[currLvl_]->getWidth() - 2);
	std::uniform_int_distribution <int> randY(1, levels_[currLvl_]->getHeight() - 2);
	
	// initializing humans
	for (int i = 0; i < levels_[currLvl_]->getNumHumans(); i++) {		
		humans_.push_back(new Human);
		auto pos = glm::vec2((float)randX(randEngine) * TILE_WITH, (float)randY(randEngine) * TILE_WITH);
		humans_.back()->init(pos, HUMAN_SPEED);
	}

	// initializing zombies
	auto zombiePositions = levels_[currLvl_]->getZombiesStartPos();
	for (size_t i = 0; i < zombiePositions.size(); i++) {
		zombies_.push_back(new Zombie);
		zombies_.back()->init(zombiePositions[i], ZOMBIE_SPEED);
	}

	// initializing guns
	player_->addGun(new Gun("Magnum", 13, 2, 0.05f, BULLET_SPEED, 7.5f, 
		audioEngine_.loadSoundEffect("Sounds/Glock.ogg")));
	player_->addGun(new Gun("Shotgun", 22, 10, 0.3f, BULLET_SPEED, 4.5f,
		audioEngine_.loadSoundEffect("Sounds/Hekler.ogg")));
	player_->addGun(new Gun("MP5", 11, 4, 0.12f, BULLET_SPEED, 3.5f,
		audioEngine_.loadSoundEffect("Sounds/Uzi.ogg")));

}

void ZombiesGame::initShaders()
{
	// adding attributes for each variable in the shader files
	// right now the entry point is the .vert file
	colorProgram_.compileShadersFromFile("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	colorProgram_.addAttribute("vertexPos");
	colorProgram_.addAttribute("vertexColor");
	colorProgram_.addAttribute("vertexUV");

	// linking the 2 shader files
	colorProgram_.linkShaders();
}

void ZombiesGame::gameLoop()
{
	// camera scaling
	camera_.setScale(CAMERA_SCALE);

	const auto DESIRED_FRAME_TIME = MILLISEC_PER_SEC / DESIRED_FPS;
	auto prevTicks = SDL_GetTicks();

	// Game loop
	while (gameState_ == GameState::PLAY) {
		fpsLimiter_.beginFrame();  // start counting fps

		auto newTicks = SDL_GetTicks();
		auto frameTicks = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = (float)frameTicks / DESIRED_FRAME_TIME;

		checkVictory();		
		inputMngr_.update();
		processInput();			 // get user input

		int timeSteps = 0; // this counter makes sure we don't spiral to death

		while (totalDeltaTime > 0.f && timeSteps < MAX_TIME_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME ); /* one step at a time */
			updateAgents(deltaTime);			// collide, move, update all game props
			updateBullets(deltaTime);

			// patricles update
			particleEngine_.update(deltaTime);
			
			totalDeltaTime -=deltaTime;
			timeSteps++;
		}
		
		// camera updating
		camera_.setPosition(player_->getPos()); // follow the player
		camera_.update();
		hudCamera_.update();
		
		drawGame(); // drawing the game

		currFps_ = fpsLimiter_.endFrame(); // returning current fps

		// print once every 10x frames
		static int frameCounter = 0;
		if (100 == frameCounter++) {
			std::cout << "fps: " << currFps_ << std::endl;
			frameCounter = 0;
		}
	}
}

/// <summary>
/// updating all agents
/// </summary>
void ZombiesGame::updateAgents(float deltaTime)
{
	// updating humans
	for (size_t i = 0; i < humans_.size(); i++) {
		humans_[i]->update(
			levels_[currLvl_]->getLevelData(), 
			humans_, 
			zombies_, deltaTime);
	}

	// updating zombies
	for (size_t i = 0; i < zombies_.size(); i++) {
		zombies_[i]->update(
			levels_[currLvl_]->getLevelData(),
			humans_,
			zombies_, deltaTime);
	}

	// updating all zombies collisions
	for (size_t i = 0; i < zombies_.size(); i++) {

		// updating collisions between zombies
		for (size_t j = i + 1; j < zombies_.size(); j++) {
			zombies_[i]->collideWithAgent(zombies_[j]);
		}

		// updating collisions with humans
		for (size_t j = 1; j < humans_.size(); j++) {
			if (zombies_[i]->collideWithAgent(humans_[j])) {
				// adding new zombie with human's position
				zombies_.push_back(new Zombie);
				zombies_.back()->init(humans_[j]->getPos(), ZOMBIE_SPEED);

				// deleting collided human
				delete humans_[j];
				humans_[j] = humans_.back();
				humans_.pop_back();
			}
		}

		// updating collisions with player
		if (zombies_[i]->collideWithAgent(player_)) {
			ge::fatalError("Game Over!");
		}
	}

	// updating collisions between humans
	for (size_t i = 0; i < humans_.size(); i++) {
		for (size_t j = i + 1; j < humans_.size(); j++) {
			humans_[i]->collideWithAgent(humans_[j]);
		}
	}
}

/// <summary>
/// updating all bullets
/// </summary>
void ZombiesGame::updateBullets(float deltaTime)
{
	for (size_t i = 0; i < bullets_.size();) {

		// collide bullets with level
		if (bullets_[i].update(levels_[currLvl_]->getLevelData(), deltaTime)) {
			bullets_[i] = bullets_.back();
			bullets_.pop_back();
		}
		else {
			i++;
		}

		// collide bullets with agents
		bool bulletRemoved = false;

		// check zombies
		for (size_t i = 0; i < bullets_.size(); i++) {
			for (size_t j = 0; j < zombies_.size();) {
				if (bullets_[i].collideWithAgent(zombies_[j])) {
					// Add blood
					addBlood(bullets_[i].getPos(), 5);

					// damaging the zombie /*true if the zombie died*/
					if (zombies_[j]->applyDamage(bullets_[i].getDamage())) {
						// deleting killed zombie
						delete zombies_[j];
						zombies_[j] = zombies_.back();
						zombies_.pop_back();
						zombiesKilled_++;
					}
					else {
						j++;
					}
					// removing the bullet
					bullets_[i] = bullets_.back();
					bullets_.pop_back();
					i--;
					bulletRemoved = true;
					break;
				}
				else {
					j++;
				}
			}
		}

		// check humans
		if (!bulletRemoved) {		
			for (size_t j = 1; j < humans_.size();) {
				if (bullets_[i].collideWithAgent(humans_[j])) {
					// Add blood
					addBlood(bullets_[i].getPos(), 5);

					// damaging the human /*true if the zombie died*/
					if (humans_[j]->applyDamage(bullets_[i].getDamage())) {
						// deleting killed human
						delete humans_[j];
						humans_[j] = humans_.back();
						humans_.pop_back();
						humansKilled_++;
					}
					else {
						j++;
					}
					// removing the bullet
					bullets_[i] = bullets_.back();
					bullets_.pop_back();
					i--;
					break;
				}
				else {
					j++;
				}
			}
		}
	}
}

void ZombiesGame::checkVictory()
{
	// TODO: Support for multiple levels
	if (zombies_.empty()) {
		std::printf("\n*** You win! ***\n You killed %d zombies_ and %d humans_.\n Saved humans_ %d\n", zombiesKilled_, humansKilled_, humans_.size() - 1);
		ge::fatalError("");
	}
}

/// <summary>
/// processing user input
/// </summary>
void ZombiesGame::processInput()
{
	// read user's input
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			gameState_ = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			inputMngr_.setMouseCoords((float)e.motion.x, (float)e.motion.y);
			break;
		case SDL_KEYDOWN:
			inputMngr_.pressKey(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputMngr_.releaseKey(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputMngr_.pressKey(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputMngr_.releaseKey(e.button.button);
			break;
		}
	}

	// execute all input actions
	if (inputMngr_.isKeyDown(SDLK_ESCAPE)) {
		gameState_ = GameState::EXIT;
	}
	if (inputMngr_.isKeyDown(SDLK_q)) {
		camera_.setScale(camera_.getScale() - 0.02f);
	}
	if (inputMngr_.isKeyDown(SDLK_e)) {
		camera_.setScale(camera_.getScale() + 0.02f);
	}

	// add, remove bullets

}

/// <summary>
/// drawing everything
/// </summary>
void ZombiesGame::drawGame()
{
	// Tells the GL the depth that should clear to
	glClearDepth(1.0);

	// To clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// draw code goes here
	colorProgram_.use();

	// this specifies which texture were binding, since multiple
	// textures can be used at the same time in the shaders
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = colorProgram_.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	// drawing / adjusting the camera here - todo - may change
	// set the camera matrix and pass it ot shader
	GLint pLocation = colorProgram_.getUniformLocation("P");
	
	// grab the camera matrix
	glm::mat4 cameraMatrix = camera_.getCameraMatrix();

	// uploading the camera matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	// drawing all game stuff

	// draw level
	levels_[currLvl_]->draw();
	
	agentSpriteBatch_.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.f);

	// draw humans, including draw player
	for (size_t i = 0; i < humans_.size(); i++) {

		if (camera_.isInView(humans_[i]->getPos(), agentDims)) {
			humans_[i]->draw(agentSpriteBatch_);
		}
	}
	
	// draw zombies
	for (size_t i = 0; i < zombies_.size(); i++) {
		if (camera_.isInView(zombies_[i]->getPos(), agentDims)) {
			zombies_[i]->draw(agentSpriteBatch_);
		}
	}

	// draw bullets
	for (size_t i = 0; i < bullets_.size(); i++) {
		bullets_[i].draw(agentSpriteBatch_);
	}

	// drawing the agents
	agentSpriteBatch_.end();
	agentSpriteBatch_.renderBatch();

	// Render the particles, reusing agents sprite batch
	particleEngine_.draw(&agentSpriteBatch_);

	drawHud();  // drawing text on the screen

	colorProgram_.unuse();

	// using double buffer rendering to avoid flickering
	window_.swapBuffer();
}

void ZombiesGame::drawHud()
{
	char buffer[256];

	// set the camera matrix and pass it ot shader
	GLint pLocation = colorProgram_.getUniformLocation("P");

	// grab the camera matrix
	glm::mat4 cameraMatrix = hudCamera_.getCameraMatrix();

	// uploading the camera matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	hudSpriteBatch_.begin();

	sprintf_s(buffer, "Num Humans %d", humans_.size());
	spriteFont_->draw(hudSpriteBatch_, buffer, glm::vec2(0, 0), 
		glm::vec2(1.0f), 0.f, ge::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "Num Zombies %d", zombies_.size());
	spriteFont_->draw(hudSpriteBatch_, buffer, glm::vec2(0, 32),
		glm::vec2(1.f), 0.f, ge::ColorRGBA8(255, 255, 255, 255));

	hudSpriteBatch_.end();
	hudSpriteBatch_.renderBatch();
}

void ZombiesGame::addBlood(const glm::vec2& position, int numParticles)
{
	// randomize shooting direction based on the spread
	static std::mt19937 randEngine(static_cast<unsigned int>(time(nullptr)));
	static std::uniform_real_distribution<float> randAngle(0.f, 360.f);

	glm::vec2 velocity(2.f, 0.0f);	
	ge::ColorRGBA8 col(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		bloodParticleBatch_->addParticle(position, glm::rotate(velocity, randAngle(randEngine)), col, 10.f);
	}
	// comment
}
