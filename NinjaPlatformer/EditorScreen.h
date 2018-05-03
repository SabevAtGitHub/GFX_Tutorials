#pragma once
#include <GameEngineOpenGL\IGameScreen.h>
#include <GameEngineOpenGL\Window.h>
#include <GameEngineOpenGL\Camera2D.h>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\SpriteFont.h>
#include <GameEngineOpenGL\GLSLProgram.h>
#include <GameEngineOpenGL\GUI.h>
#include "ScreenIndices.h"

class EditorScreen : public ge::IGameScreen
{
public:
	EditorScreen(ge::Window* window);
	~EditorScreen();

#pragma region TODO: TEST GUI ELEMENTS

	// Inherited via IGameScreen
	virtual int getNextScreenIndex() const override;

	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;

	virtual void destroy() override;

	virtual void onEntry() override;

	virtual void onExit() override;

	virtual void update() override;

	virtual void draw() override;

#pragma endregion TODO: TEST GUI ELEMENTS

	void initShaders();
	
private:
	void initGUI();
	void checkInput();
	bool onExitClicked(const CEGUI::EventArgs& eargs);
	bool onRedPickerChanged();
	bool onGreenPickerChanged();
	bool onBluePickerChanged();

private:
	ge::Window* m_window = nullptr;
	ge::GUI m_gui;
	ge::Camera2D m_camera; // Renders the scene
	ge::SpriteBatch m_spriteBatch;
	ge::SpriteFont m_spriteFont;
	ge::GLSLProgram m_textureProgram;// Shader for the textures
	GLuint m_blankTexture = 0;

	float m_rColorVal = 255.f;
	float m_gColorVal = 0.f;
	float m_bColorVal = 128.f;

	CEGUI::Slider* m_rSlider = nullptr;
	CEGUI::Slider* m_gSlider = nullptr;
	CEGUI::Slider* m_bSlider = nullptr;
};

