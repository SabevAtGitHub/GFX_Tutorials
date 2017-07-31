#include "MainGame.h"
#include <iostream>
#include <string>
#include <GameEngineOpenGL\ErrManager.h>
#include <GameEngineOpenGL\GameEngineOpenGL.h>
#include <GameEngineOpenGL\ResourceManager.h>


MainGame::MainGame() :
	time(0),
	scrWidth(900.f),
	scrHeight(600.f),
	gameState(GameState::PLAY),
	maxFps(60.0f)
{
	///< TODO - to recognize user's screen resolution and
	/// change w and h to variables
	
}

MainGame::~MainGame() { }

/// <summary>
/// running the game
/// </summary>
void MainGame::run()
{
	initSystems();

	gameLoop();
}

/// <summary>
/// Opens the game window
/// initializing all systems ...
/// todo - to replace above line with all syst
/// em's names 
/// </summary>
void MainGame::initSystems()
{
	// initiating SDL engine
	ge::init(); // SDL_Init(SDL_INIT_EVERYTHING);

	// set up the camera
	camera.init(scrWidth, scrHeight);

	/// Creating the main window
	window_.create("Game Window", scrWidth, scrHeight, ge::WINDOW_SHOWN);
	
	// Calling program to compile the shaders
	initShaders();
	spriteBatch.init();
	fpsLimiter.setTargetFps(maxFps);
}

/// <summary>
/// Processing user's unput
/// </summary>
void MainGame::processInput()
{
	const float CAMERA_SPEED = 2.0f;
	const float CAMERA_SCALE = 0.2f;

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			gameState = GameState::EXIT;
		case SDL_MOUSEMOTION:
			inputManager.setMouseCoords((float)e.motion.x, (float)e.motion.y);
			break;
		case SDL_KEYDOWN:
			inputManager.pressKey(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager.releaseKey(e.key.keysym.sym);			 
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.pressKey(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.releaseKey(e.button.button);
			break;
		}
	}

	// processing input - todo - move it from here someday
	if (inputManager.isKeyDown(SDLK_w)) {
		camera.setPosition(camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (inputManager.isKeyDown(SDLK_s)) {
		camera.setPosition(camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (inputManager.isKeyDown(SDLK_d)) {
		camera.setPosition(camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (inputManager.isKeyDown(SDLK_a)) {
		camera.setPosition(camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (inputManager.isKeyDown(SDLK_q)) {
		camera.setScale(camera.getScale() - CAMERA_SCALE);
	}
	if (inputManager.isKeyDown(SDLK_e)) {
		camera.setScale(camera.getScale() + CAMERA_SCALE);
	}

	if (inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = inputManager.getMouseCoords();
		mouseCoords = camera.covertScreenToWorld(mouseCoords);

		//std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		// determining bullet's direction
		glm::vec2 playerPos(38.f, 22.f);
		glm::vec2 direction = mouseCoords - playerPos;
		direction = glm::normalize(direction);
	
		// adding bullets
		bullets.emplace_back(playerPos, direction, 1.5f, 1000);
	}
}

/// <summary>
/// All the rendering code is here
/// <summary>
void MainGame::drawGame()
{
	// Tells the GL the depth that should clear to
	glClearDepth(1.0);

	// To clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	colorProgram.use();

	// this specifies which texture were binding, since multiple
	// textures can be used at the same time in the shaders
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	// getting the location of the uniform variable in the shader program
	// set the camera matrix and pass it ot shader
	GLint pLocation = colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	
	// uploading the camera matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	// using the spriteBatch to ...
	spriteBatch.begin();

	// temp object to pass to draw function - todo - to replace it
	// with actual game object in the future
	glm::vec4 pos(0.f, 0.f, 50.f, 50.f);
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);

	static auto texture = ge::ResourceManager::getTexture(
		"Textures/JimmyJump_pack/PNG/CharacterRight_Standing.png");
	auto color = ge::ColorRGBA8(255, 255, 255, 255);

	// drawing sprites
	spriteBatch.draw(pos, uv, texture.id, 0.f /* depth */, color);

	// drawing bullets
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].draw(spriteBatch);
	}

	// closing the batch and rendering
	spriteBatch.end();
	spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	colorProgram.unuse();

	window_.swapBuffer();
}

/// <summary>
/// The main function of the MainGame
/// Looping while m_gameState != EXIT
/// </summary>
void MainGame::gameLoop()
{
	while (gameState != GameState::EXIT) {
		fpsLimiter.beginFrame(); // will get initial frame ticks

		processInput();
		//time += 0.002f; // todo - guess and check
		camera.update();

		// updating bullets
		for (size_t i = 0; i < bullets.size();) {
			if (bullets[i].update()) {
				bullets[i] = bullets.back();
				bullets.pop_back();
			}
			else {
				i++;
			}
		}

		drawGame();
		
		fps = fpsLimiter.endFrame(); // returning current fps

		// print once every 10x frames
		static int frameCounter = 0;
		frameCounter++;
		if (100 == frameCounter) {
			std::cout << fps << std::endl;
			frameCounter = 0;
		}
	}
}

void MainGame::initShaders()
{
	// adding attributes for each variable in the shader files
	// right now the entry point is the .vert file
	colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	colorProgram.addAttribute("vertexPos");
	colorProgram.addAttribute("vertexColor");
	colorProgram.addAttribute("vertexUV");

	// linking the 2 shader files
	colorProgram.linkShaders();
}


