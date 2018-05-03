#pragma once
#include <GameEngineOpenGL\IGameScreen.h>
#include <GameEngineOpenGL\Window.h>
#include <GameEngineOpenGL\Camera2D.h>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <GameEngineOpenGL\SpriteFont.h>
#include <GameEngineOpenGL\GLSLProgram.h>
#include <GameEngineOpenGL\GLTexture.h>
#include <GameEngineOpenGL\GUI.h>
#include "ScreenIndices.h"


enum class PhysicsMode {
	RIGID, DYNAMIC,
};

class EditorScreen : public ge::IGameScreen
{
public:
	EditorScreen(ge::Window* window);
	~EditorScreen();

#pragma region Inherited via IGameScreen

	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;
	virtual void build() override;
	virtual void destroy() override;
	virtual void onEntry() override;
	virtual void onExit() override;
	virtual void update() override;
	virtual void draw() override;

#pragma endregion Inherited via IGameScreen

	void initShaders();
	
private:
	void initGUI();
	void checkInput();
	bool onExitClicked(const CEGUI::EventArgs& eargs);
	bool onRedPickerChanged(const CEGUI::EventArgs& eargs);
	bool onGreenPickerChanged(const CEGUI::EventArgs& eargs);
	bool onBluePickerChanged(const CEGUI::EventArgs& eargs);
	bool onRigidMouseClick(const CEGUI::EventArgs& eargs);
	bool onDynamicMouseClick(const CEGUI::EventArgs& eargs);

private:
	ge::Window* m_window = nullptr;
	ge::GUI m_gui;
	ge::Camera2D m_camera; // Renders the scene
	ge::SpriteBatch m_spriteBatch;
	ge::SpriteFont m_spriteFont;
	ge::GLSLProgram m_textureProgram;// Shader for the textures
	ge::GLTexture m_blankTexture;

	PhysicsMode m_physicsMode = PhysicsMode::RIGID;

	float m_rColorVal = 255.f;
	float m_gColorVal = 255.f;
	float m_bColorVal = 0.0f;

	CEGUI::Slider* m_rSlider = nullptr;
	CEGUI::Slider* m_gSlider = nullptr;
	CEGUI::Slider* m_bSlider = nullptr;
	CEGUI::RadioButton* m_rigidRadioBtn = nullptr;
	CEGUI::RadioButton* m_dynamicRadioBtn = nullptr;
};

