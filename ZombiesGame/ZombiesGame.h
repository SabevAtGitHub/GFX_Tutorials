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
	ge::Window m_window;
	ge::Camera2D m_camera2D;
	ge::Camera2D m_hudCamera;
	ge::SpriteBatch m_agentSpriteBatch; /// to draw all agents
	ge::SpriteBatch m_hudSpriteBatch; /// to draw hud text
	ge::ParticleEngine2D particleEngine_;
	ge::ParticleBatch2D* m_bloodParticleBatch = nullptr;

	ge::InputManager m_inputManager;
	ge::FpsLimiter m_fpsLimiter;
	GameState m_gameState;		  // enum class in this header
	ge::GLSLProgram m_colorProgram; // used in void initShaders
	ge::SpriteFont* m_spriteFont = nullptr;
	ge::AudioManager m_audioEngine;
	
	int scrW, scrH;
	float m_maxFps, m_currFps;

	std::vector<Level*> m_levels;
	int m_currLvl;

	Player* m_player = nullptr;
	std::vector<Human*> m_humans; ///< vector of all humans
	std::vector<Zombie*> m_zombies; ///< vector of all zombies
	std::vector<Bullet> m_bullets; ///< vector of all bullets

	int m_humansKilled;
	int m_zombiesKilled;
};

