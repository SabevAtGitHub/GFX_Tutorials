#include "GameplayScreen.h"
#include <GameEngineOpenGL\IMainGame.h>
#include <iostream>
#include <SDL\SDL.h>

GameplayScreen::GameplayScreen()
{
}


GameplayScreen::~GameplayScreen()
{
}

int GameplayScreen::getNextScreenIndex() const
{
	return NO_NEXT_SCREEN_INDEX;
}

int GameplayScreen::getPreviousScreenIndex() const
{
	return NO_NEXT_SCREEN_INDEX;
}

void GameplayScreen::build()
{
}

void GameplayScreen::destroy()
{
}

void GameplayScreen::onEntry()
{
	b2Vec2 gravity(0.f, -9.81f);
	m_world = std::make_unique<b2World>(gravity);

	// Creating the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.f, -10.f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	// Creating the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.f, 10.f);
	groundBody->CreateFixture(&groundBox, 0.f);


}

void GameplayScreen::onExit()
{

}

void GameplayScreen::update()
{
	std::cout << "update\n";
	checkInput();
}

void GameplayScreen::draw()
{
	std::cout << "Draw\n";
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.f, 0.f, 0.f, 1.f);
}

void GameplayScreen::checkInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}
