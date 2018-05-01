#pragma once
#include <vector>
#include <GameEngineOpenGL\IGameScreen.h>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\GLTexture.h>
#include <GameEngineOpenGL\Window.h>
#include <GameEngineOpenGL\GLSLProgram.h>
#include <GameEngineOpenGL\DebugRenderer.h>
#include <GameEngineOpenGL\Camera2D.h>
#include <GameEngineOpenGL\GUI.h>
#include <Box2D\Box2D.h>
#include "Box.h"
#include "Player.h"
#include "Light.h"


class GameplayScreen : public ge::IGameScreen
{
public:
	GameplayScreen(ge::Window* window);
	~GameplayScreen();
	
#pragma region Inherited

	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;
	virtual void build() override;
	virtual void destroy() override;
	virtual void onEntry() override;
	virtual void onExit() override;
	virtual void update() override;
	virtual void draw() override;

#pragma endregion // Inherited

private:

	void initGUI();
	void initGraphics();
	void initShaders();
	void initLights();
	void initActors();
	void spawnBoxes(int numBoxes);
	void checkInput();

private:

	Player m_player;
	Light m_playerLight;
	Light m_mouseLight;

	std::vector<Box> m_boxes;
	std::unique_ptr<b2World> m_world = nullptr;
	ge::SpriteBatch m_spriteBatch; 	
	ge::Camera2D m_camera; // Renders the scene
	ge::GLTexture m_boxTexture2D; 
	ge::Window* m_window = nullptr;	
	ge::GLSLProgram m_textureProgram;// Shader for the textures	
	ge::GLSLProgram m_lightProgram;// Shader for the lights
	ge::DebugRenderer m_debugRenderer;
	ge::GUI m_gui;

	bool m_renderDebug = false;
};

