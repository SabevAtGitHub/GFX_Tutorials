#pragma once
#include <GameEngineOpenGL\IMainGame.h>
#include <memory>
#include "GameplayScreen.h"

class App : public ge::IMainGame
{
public:
	App();
	~App();
#pragma region Inherited via IGameScreen

	// Called on initialization
	virtual void onInit() override;

	// For adding all screens
	virtual void addScreens() override;

	// Called when exiting
	virtual void onExit() override;

#pragma endregion // Inherited via IGameScreen

private:
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
};

