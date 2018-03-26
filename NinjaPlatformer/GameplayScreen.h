#pragma once
#include <GameEngineOpenGL\IGameScreen.h>
#include <Box2D\Box2D.h>

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

private:

	std::unique_ptr<b2World> m_world = nullptr;

};

