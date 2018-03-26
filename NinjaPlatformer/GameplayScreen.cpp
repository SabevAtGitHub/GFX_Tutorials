#include "GameplayScreen.h"
#include <GameEngineOpenGL\ResourceManager.h>


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

	// Make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.f, -20.f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	// Make ground fixture
	b2PolygonShape groundBoxShape;
	groundBoxShape.SetAsBox(50.f, 10.f);
	groundBody->CreateFixture(&groundBoxShape, 0.f);

	Box newBox;
	newBox.init(m_world.get(), glm::vec2(0.f, 14.f), glm::vec2(15.f, 15.f));

	m_boxes.push_back(newBox);

	// Initialize spriteBatch
	m_spriteBatch.init();
}

void GameplayScreen::onExit()
{
}

void GameplayScreen::update()
{
	checkInput();
}

void GameplayScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.f, 0.f, 1.f); // red

	m_spriteBatch.begin();



	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}

void GameplayScreen::checkInput()
{
}
