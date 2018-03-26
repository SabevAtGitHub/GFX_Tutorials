#pragma once
#include <GameEngineOpenGL\IMainGame.h>
#include "GameplayScreen.h"

class App : public ge::IMainGame
{
public:
	App();
	~App();

#pragma region Inherited via IMainGame

	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;

#pragma endregion // Inherited via IMainGame
private:
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;


};

