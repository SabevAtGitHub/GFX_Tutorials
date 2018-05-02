#pragma once
#include <GameEngineOpenGL\IGameScreen.h>
#include <GameEngineOpenGL\Window.h>
#include <GameEngineOpenGL\Camera2D.h>
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
	
private:
	void initGUI();
	void checkInput();
	bool onExitClicked(const CEGUI::EventArgs& eargs);

private:

	ge::Window* m_window = nullptr;
	ge::GUI m_gui;
	ge::Camera2D m_camera; // Renders the scene
	//int m_nextScreenIdx = SCREEN_INDEX_GAMEPLAY;
	float m_colorPickerRed = 255.f;
	float m_colorPickerGreen = 255.f;
	float m_colorPickerBlue = 255.f;
};

