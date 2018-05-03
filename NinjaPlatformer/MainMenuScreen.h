#pragma once
#include <GameEngineOpenGL\IGameScreen.h>
#include <GameEngineOpenGL\Window.h>
#include <GameEngineOpenGL\Camera2D.h>
#include <GameEngineOpenGL\GUI.h>
#include "ScreenIndices.h"

class MainMenuScreen : public ge::IGameScreen
{
public:
	MainMenuScreen(ge::Window* window);
	~MainMenuScreen();

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

private:
	void initGUI();
	void checkInput();
	bool onPlayGameClicked(const CEGUI::EventArgs& eargs);
	bool onEditorClicked(const CEGUI::EventArgs& eargs);
	bool onExitClicked(const CEGUI::EventArgs& eargs);

private:
	ge::Window* m_window = nullptr;
	ge::GUI m_gui;
	ge::Camera2D m_camera; // Renders the scene
	int m_nextScreenIdx = SCREEN_INDEX_GAMEPLAY;
};

