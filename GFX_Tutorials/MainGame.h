#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <GameEngineOpenGL\Sprite.h>
#include <GameEngineOpenGL\GLSLProgram.h>
#include <GameEngineOpenGL\GLTexture.h>
#include <GameEngineOpenGL\Window.h>
#include <GameEngineOpenGL\Camera2D.h>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\InputManager.h>
#include <GameEngineOpenGL\Timing.h>
#include <vector>
#include "Bullet.h"

enum class GameState { PLAY, EXIT };

/// <summary>
/// the game
/// </summary>
class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();	

private:
	void initSystems();
	void processInput();
	void drawGame();
	void gameLoop();
	void initShaders();
	
	///<summary>
	/// the game window
	/// </summary>
	ge::Window window_; 

	float scrWidth, scrHeight;

	GameState gameState;
	
	ge::GLSLProgram colorProgram;
	ge::Camera2D camera;
	ge::SpriteBatch spriteBatch;
	ge::InputManager inputManager;
	ge::FpsLimiter fpsLimiter;
	std::vector<Bullet> bullets;

	float time;
	float fps;
	const float maxFps;
	
};

