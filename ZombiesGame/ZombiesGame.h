#pragma once

#include <GameEngineOpenGL\Window.h>
#include <GameEngineOpenGL\Camera2D.h>
#include <GameEngineOpenGL\InputManager.h>
#include <GameEngineOpenGL\GLSLProgram.h>
#include <GameEngineOpenGL\Timing.h>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\SpriteFont.h>
#include <GameEngineOpenGL\AudioManager.h>
#include <GameEngineOpenGL\ParticleEngine2D.h>
#include <GameEngineOpenGL\ParticleBatch2D.h>

#include "Level.h"
#include "Player.h"

enum class GameState { PLAY, EXIT };

/// <summary>
/// the game
/// </summary>

class ZombiesGame
{
public:
	ZombiesGame();
	~ZombiesGame();
	void run();
	void initSystems();
	void initGameProps();
	void initShaders();
	void gameLoop();
	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);
	void checkVictory();
	void processInput();
	void drawGame();
	void drawHud();
	void addBlood(const glm::vec2& position, int numParticles);

private:
	ge::Window window_;
	ge::Camera2D camera_;
	ge::Camera2D hudCamera_;
	ge::SpriteBatch agentSpriteBatch_; /// to draw all agents
	ge::SpriteBatch hudSpriteBatch_; /// to draw hud text
	ge::ParticleEngine2D particleEngine_;
	ge::ParticleBatch2D* bloodParticleBatch_ = nullptr;

	ge::InputManager inputMngr_;
	ge::FpsLimiter fpsLimiter_;
	GameState gameState_;		  // class in this header
	ge::GLSLProgram colorProgram_; // used in void initShaders
	ge::SpriteFont* m_spriteFont = nullptr;
	ge::AudioManager audioEngine_;
	
	int scrW_, scrH_;
	float maxFps_, currFps_;

	std::vector<Level*> levels_;
	int currLvl_;

	Player* player_ = nullptr;
	std::vector<Human*> humans_; ///< vector of all humans
	std::vector<Zombie*> zombies_; ///< vector of all zombies
	std::vector<Bullet> bullets_; ///< vector of all bullets

	int humansKilled_;
	int zombiesKilled_;
};

