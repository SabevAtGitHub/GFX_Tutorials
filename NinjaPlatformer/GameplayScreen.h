#pragma once
#include <vector>
#include <GameEngineOpenGL\IGameScreen.h>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\GLTexture.h>
#include <GameEngineOpenGL\Window.h>
#include <GameEngineOpenGL\GLSLProgram.h>
#include <GameEngineOpenGL\DebugRenderer.h>
#include <GameEngineOpenGL\Camera2D.h>
#include <Box2D\Box2D.h>
#include "Box.h"
#include "Player.h"

class GameplayScreen : public ge::IGameScreen
{
public:
	GameplayScreen(ge::Window* window);
	~GameplayScreen();
	
#pragma region Inherited via IGameScreen

	virtual int getNextScreenIndex() const override;

	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;

	virtual void destroy() override;

	virtual void onEntry() override;

	virtual void onExit() override;

	virtual void update() override;

	virtual void draw() override;

#pragma endregion // Inherited via IGameScreen

private:

	void checkInput();
	void initShaders();
	void spawnBoxes(int numBoxes);

private:

	Player m_player;
	std::vector<Box> m_boxes;
	std::unique_ptr<b2World> m_world = nullptr;

	ge::SpriteBatch m_spriteBatch; 

	// Renders the scene
	ge::Camera2D m_camera; 

	ge::GLTexture m_boxTexture2D; 

	ge::Window* m_window = nullptr;

	// Shader for the textures
	ge::GLSLProgram m_textureProgram;

	// Shader for the lights
	ge::GLSLProgram m_lightProgram;

	ge::DebugRenderer m_debugRenderer;

	bool m_renderDebug = false;
};

