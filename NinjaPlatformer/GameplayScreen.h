#pragma once
#include <vector>
#include <GameEngineOpenGL\IGameScreen.h>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\GLSLProgram.h>
#include <GameEngineOpenGL\Camera2D.h>
#include <Box2D\Box2D.h>
#include "Box.h"

class GameplayScreen : public ge::IGameScreen
{
public:
	GameplayScreen();
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

	std::vector<Box> m_boxes;
	std::unique_ptr<b2World> m_world = nullptr;

private:
	ge::SpriteBatch m_spriteBatch; ///< Renders all the balls
	ge::Camera2D m_camera; ///< Renders the scene
	ge::GLSLProgram m_textureProgram; ///< Shader for textures]
};

