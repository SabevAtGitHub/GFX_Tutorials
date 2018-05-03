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
	m_gameState(GameState::PLAY),
	scrW(1366),
	scrH(768),
	m_maxFps(60.f),
	m_currFps(0),
	m_player(nullptr),
	m_humansKilled(0),
	m_zombiesKilled(0)
{ /* empty */ }

// destructor
ZombiesGame::~ZombiesGame() { 

	for (size_t i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}

	// destroy humans
	for (size_t i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	} 

	for (size_t i = 0; i < m_zombies.size(); i++) {
		delete m_zombies[i];
	}

	delete m_spriteFont;
}

void ZombiesGame::run()
{
	initSystems();
	initGameProps();

	auto music = m_audioEngine.loadMusic("Sounds/Suspense Loop.wav");
	music.play(-1);

	gameLoop();
}

void ZombiesGame::initSystems()
{
	// calling SDL_Init(SDL_INIT_EVERYTHING);
	ge::init();

	// initializing sound, must happen after ge::init()
	m_audioEngine.init();

	// set up the camera
	m_camera2D.init(scrW, scrH);
	m_hudCamera.init(scrW, scrH);
	m_hudCamera.setPosition(glm::vec2(scrW / 2, scrH / 2));

	// create the window
	m_window.create("Zombies Game", scrW, scrH, ge::WINDOW_SHOWN);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // light gray background

	// Calling program to compile the shaders
	initShaders();
	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	// initializing sprite font 
	// ( must be initialized after initializing SDL, OpenGL and shaders )
	m_spriteFont = new ge::SpriteFont("Fonts/chintzy.ttf", 31);

	// initializing particles
	m_bloodParticleBatch = new ge::ParticleBatch2D();
	m_bloodParticleBatch->init( 1000, 0.05f, 
		ge::ResourceManager::getTexture("Textures/particle.png"),
		[] (ge::Particle2D& p, float deltaTime)  
			{ p.pos += p.velocity * deltaTime; 
			  p.color.a = (GLubyte)(p.life * 255.f); }); // lampda for blood particles custom behavior

	m_particleEngine.addParticleBatch(m_bloodParticleBatch);

	// setting the FPS limiter
	m_fpsLimiter.setTargetFps(DESIRED_FPS);
}

void ZombiesGame::initGameProps()
{
	// initializing level 1
	m_levels.push_back(new Level("Levels/level_1.txt"));
	m_currLvl = 0;

	// initializing player
	m_player = new Player(
		m_levels[m_currLvl]->getPlayerStartPos(), 
		glm::vec2(0.f, 1.f), PLAYER_SPEED, 3, 
		&m_inputManager, &m_camera2D, &m_bullets);
	
	// Add player to vecor of humans
	m_humans.push_back(m_player);

	// random POS to use when initializing humans
	std::mt19937 randEngine;
	randEngine.seed(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution <int> randX(1, m_levels[m_currLvl]->getWidth() - 2);
	std::uniform_int_distribution <int> randY(1, m_levels[m_currLvl]->getHeight() - 2);
	
	// initializing humans
	for (int i = 0; i < m_levels[m_currLvl]->getNumHumans(); i++) {		
		m_humans.push_back(new Human);
		auto pos = glm::vec2((float)randX(randEngine) * TILE_WITH, (float)randY(randEngine) * TILE_WITH);
		m_humans.back()->init(pos, HUMAN_SPEED);
	}

	// initializing zombies
	auto zombiePositions = m_levels[m_currLvl]->getZombiesStartPos();
	for (size_t i = 0; i < zombiePositions.size(); i++) {
		m_zombies.push_back(new Zombie);
		m_zombies.back()->init(zombiePositions[i], ZOMBIE_SPEED);
	}

	// initializing guns
	m_player->addGun(new Gun("Magnum", 13, 2, 0.05f, BULLET_SPEED, 7.5f, 
		m_audioEngine.loadSoundEffect("Sounds/Glock.ogg")));
	m_player->addGun(new Gun("Shotgun", 22, 10, 0.3f, BULLET_SPEED, 4.5f,
		m_audioEngine.loadSoundEffect("Sounds/Hekler.ogg")));
	m_player->addGun(new Gun("MP5", 11, 4, 0.12f, BULLET_SPEED, 3.5f,
		m_audioEngine.loadSoundEffect("Sounds/Uzi.ogg")));

}

void ZombiesGame::initShaders()
{
	// adding attributes for each variable in the shader files
	// right now the entry point is the .vert file
	m_colorProgram.compileShadersFromFile("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_colorProgram.addAttribute("vertexPos");
	m_colorProgram.addAttribute("vertexColor");
	m_colorProgram.addAttribute("vertexUV");

	// linking the 2 shader files
	m_colorProgram.linkShaders();
}

void ZombiesGame::gameLoop()
{
	// camera scaling
	m_camera2D.setScale(CAMERA_SCALE);

	const auto DESIRED_FRAME_TIME = MILLISEC_PER_SEC / DESIRED_FPS;
	auto prevTicks = SDL_GetTicks();

	// Game loop
	while (m_gameState == GameState::PLAY) {
		m_fpsLimiter.beginFrame();  // start counting fps

		auto newTicks = SDL_GetTicks();
		auto frameTicks = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = (float)frameTicks / DESIRED_FRAME_TIME;

		checkVictory();		
		m_inputManager.update();
		processInput();			 // get user input

		int timeSteps = 0; // this counter makes sure we don't spiral to death

		while (totalDeltaTime > 0.f && timeSteps < MAX_TIME_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME ); /* one step at a time */
			updateAgents(deltaTime);			// collide, move, update all game props
			updateBullets(deltaTime);

			// patricles update
			m_particleEngine.update(deltaTime);
			
			totalDeltaTime -=deltaTime;
			timeSteps++;
		}
		
		// camera updating
		m_camera2D.setPosition(m_player->getPos()); // follow the player
		m_camera2D.update();
		m_hudCamera.update();
		
		drawGame(); // drawing the game

		m_currFps = m_fpsLimiter.endFrame(); // returning current fps

		// print once every 10x frames
		static int frameCounter = 0;
		if (100 == frameCounter++) {
			std::cout << "fps: " << m_currFps << std::endl;
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
	for (size_t i = 0; i < m_humans.size(); i++) {
		m_humans[i]->update(
			m_levels[m_currLvl]->getLevelData(), 
			m_humans, 
			m_zombies, deltaTime);
	}

	// updating zombies
	for (size_t i = 0; i < m_zombies.size(); i++) {
		m_zombies[i]->update(
			m_levels[m_currLvl]->getLevelData(),
			m_humans,
			m_zombies, deltaTime);
	}

	// updating all zombies collisions
	for (size_t i = 0; i < m_zombies.size(); i++) {

		// updating collisions between zombies
		for (size_t j = i + 1; j < m_zombies.size(); j++) {
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}

		// updating collisions with humans
		for (size_t j = 1; j < m_humans.size(); j++) {
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
				// adding new zombie with human's position
				m_zombies.push_back(new Zombie);
				m_zombies.back()->init(m_humans[j]->getPos(), ZOMBIE_SPEED);

				// deleting collided human
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}

		// updating collisions with player
		if (m_zombies[i]->collideWithAgent(m_player)) {
			ge::fatalError("Game Over!");
		}
	}

	// updating collisions between humans
	for (size_t i = 0; i < m_humans.size(); i++) {
		for (size_t j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}
}

/// <summary>
/// updating all bullets
/// </summary>
void ZombiesGame::updateBullets(float deltaTime)
{
	for (size_t i = 0; i < m_bullets.size();) {

		// collide bullets with level
		if (m_bullets[i].update(m_levels[m_currLvl]->getLevelData(), deltaTime)) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}

		// collide bullets with agents
		bool bulletRemoved = false;

		// check zombies
		for (size_t i = 0; i < m_bullets.size(); i++) {
			for (size_t j = 0; j < m_zombies.size();) {
				if (m_bullets[i].collideWithAgent(m_zombies[j])) {
					// Add blood
					addBlood(m_bullets[i].getPos(), 5);

					// damaging the zombie /*true if the zombie died*/
					if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {
						// deleting killed zombie
						delete m_zombies[j];
						m_zombies[j] = m_zombies.back();
						m_zombies.pop_back();
						m_zombiesKilled++;
					}
					else {
						j++;
					}
					// removing the bullet
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
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
			for (size_t j = 1; j < m_humans.size();) {
				if (m_bullets[i].collideWithAgent(m_humans[j])) {
					// Add blood
					addBlood(m_bullets[i].getPos(), 5);

					// damaging the human /*true if the zombie died*/
					if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
						// deleting killed human
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
						m_humansKilled++;
					}
					else {
						j++;
					}
					// removing the bullet
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
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
	if (m_zombies.empty()) {
		std::printf("\n*** You win! ***\n You killed %d m_zombies and %d m_humans.\n Saved m_humans %d\n", m_zombiesKilled, m_humansKilled, m_humans.size() - 1);
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
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords((float)e.motion.x, (float)e.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(e.button.button);
			break;
		}
	}

	// execute all input actions
	if (m_inputManager.isKeyDown(SDLK_ESCAPE)) {
		m_gameState = GameState::EXIT;
	}
	if (m_inputManager.isKeyDown(SDLK_q)) {
		m_camera2D.setScale(m_camera2D.getScale() - 0.02f);
	}
	if (m_inputManager.isKeyDown(SDLK_e)) {
		m_camera2D.setScale(m_camera2D.getScale() + 0.02f);
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
	m_colorProgram.use();

	// this specifies which texture were binding, since multiple
	// textures can be used at the same time in the shaders
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = m_colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	// drawing / adjusting the camera here - todo - may change
	// set the camera matrix and pass it ot shader
	GLint pLocation = m_colorProgram.getUniformLocation("P");
	
	// grab the camera matrix
	glm::mat4 cameraMatrix = m_camera2D.getCameraMatrix();

	// uploading the camera matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	// drawing all game stuff

	// draw level
	m_levels[m_currLvl]->draw();
	
	m_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.f);

	// draw humans, including draw player
	for (size_t i = 0; i < m_humans.size(); i++) {

		if (m_camera2D.isInView(m_humans[i]->getPos(), agentDims)) {
			m_humans[i]->draw(m_agentSpriteBatch);
		}
	}
	
	// draw zombies
	for (size_t i = 0; i < m_zombies.size(); i++) {
		if (m_camera2D.isInView(m_zombies[i]->getPos(), agentDims)) {
			m_zombies[i]->draw(m_agentSpriteBatch);
		}
	}

	// draw bullets
	for (size_t i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	// drawing the agents
	m_agentSpriteBatch.end();
	m_agentSpriteBatch.renderBatch();

	// Render the particles, reusing agents sprite batch
	m_particleEngine.draw(&m_agentSpriteBatch);

	drawHud();  // drawing text on the screen

	m_colorProgram.unuse();

	// using double buffer rendering to avoid flickering
	m_window.swapBuffer();
}

void ZombiesGame::drawHud()
{
	char buffer[256];

	// set the camera matrix and pass it ot shader
	GLint pLocation = m_colorProgram.getUniformLocation("P");

	// grab the camera matrix
	glm::mat4 cameraMatrix = m_hudCamera.getCameraMatrix();

	// uploading the camera matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_hudSpriteBatch.begin();

	sprintf_s(buffer, "Num Humans %d", m_humans.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 0), 
		glm::vec2(1.0f), 0.f, ge::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "Num Zombies %d", m_zombies.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 32),
		glm::vec2(1.f), 0.f, ge::ColorRGBA8(255, 255, 255, 255));

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}

void ZombiesGame::addBlood(const glm::vec2& position, int numParticles)
{
	// randomize shooting direction based on the spread
	static std::mt19937 randEngine(static_cast<unsigned int>(time(nullptr)));
	static std::uniform_real_distribution<float> randAngle(0.f, 360.f);

	glm::vec2 velocity(2.f, 0.0f);	
	ge::ColorRGBA8 col(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		m_bloodParticleBatch->addParticle(position, glm::rotate(velocity, randAngle(randEngine)), col, 10.f);
	}
	// comment
}
