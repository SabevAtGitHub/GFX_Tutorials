#include "GameplayScreen.h"
#include <GameEngineOpenGL\ResourceManager.h>
#include <GameEngineOpenGL\IMainGame.h>
#include <GameEngineOpenGL\Vertex.h>
#include <random>

GameplayScreen::GameplayScreen(ge::Window* window)
	: m_window(window)
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
	static const float CAMERA_SCALE = 30.f;
	static const float WORLD_FLOOR_HEIGHT = -20.f;
	static const float GRAVITY_RATE = -13.81f;
	static const int NUM_BOXES = 7;
	static const float PLAYER_W = 1.0f, PLAYER_H = 2.0f;

	b2Vec2 gravity(0.f, GRAVITY_RATE);
	m_world = std::make_unique<b2World>(gravity);
	m_debugRenderer.init();

	// Make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.f, WORLD_FLOOR_HEIGHT);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	// Make ground fixture
	b2PolygonShape groundBoxShape;
	groundBoxShape.SetAsBox(50.f, 10.f);
	groundBody->CreateFixture(&groundBoxShape, 0.f);

	// creating number of boxes
	spawnBoxes(NUM_BOXES);

	// Initialize spriteBatch
	m_spriteBatch.init();

	initShaders();

	// init player
	auto pos = glm::vec2(2.f, 25.f);
	auto color = ge::ColorRGBA8(255, 255, 255, 255);
	auto drawDims = glm::vec2(2.5f);
	auto collisionDims = glm::vec2(1.25f, 2.3f);

	m_player.init(m_world.get(), pos, drawDims, collisionDims, color, true);

	// Init the camera
	m_camera.init(m_window->getWidth(), m_window->getHeight());
	m_camera.setScale(CAMERA_SCALE);
}

void GameplayScreen::onExit()
{
	m_debugRenderer.dispose();
}

void GameplayScreen::update()
{
	m_camera.update();
	checkInput();
	m_player.update(m_game->inputManager);

	// updating physics simulation
	m_world->Step(1.f / 60.f, 6, 2);
}

void GameplayScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.f, 0.f, 1.f); // red

	m_textureProgram.use();

	// this specifies which texture were binding, since multiple
	// textures can be used at the same time in the shaders
	GLint textureLocation = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);

	// grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();

	// drawing / adjusting the camera here - todo - may change
	// set the camera matrix and pass it ot shader
	GLint pLocation = m_textureProgram.getUniformLocation("P");

	// uploading the camera matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	//// Upload texture uniform
	//auto textureUniform = m_textureProgram.getUniformLocation(pLocation);
	m_spriteBatch.begin();

	// draw all boxes
	for (auto& b : m_boxes) {
		b.draw(m_spriteBatch);
	}

	m_player.draw(m_spriteBatch);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();

	// Debug rendering...
	if (m_renderDebug) {
		auto color = ge::ColorRGBA8(255, 255, 255, 255);
		// ...boxes
		for (auto& b : m_boxes) {
			m_debugRenderer.drawBox(b.getDestRect(), color, b.getBody()->GetAngle());
		}

		// ...player
		m_player.drawDebug(m_debugRenderer);

		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.f);
	}
}

void GameplayScreen::checkInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}

void GameplayScreen::initShaders()
{
	// adding attributes for each variable in the shader files
	// right now the entry point is the .vert file
	m_textureProgram.compileShadersFromFile(std::string("Shaders/colorShading.vert"),
		std::string("Shaders/colorShading.frag"));
	m_textureProgram.addAttribute("vertexPos");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");

	// linking the 2 shader files
	m_textureProgram.linkShaders();
}

// creating number of boxes
void GameplayScreen::spawnBoxes(int numBoxes)
{
	std::mt19937 rng;

	std::uniform_real_distribution<float> xPos(-10.f, 10.f);
	std::uniform_real_distribution<float> yPos(-10.f, 15.f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);
	std::uniform_int_distribution<int> color(0, 225);

	m_boxTexture2D = ge::ResourceManager::getTexture("Assets/bricks_top.png");

	for (int i = 0; i < numBoxes; i++) {

		Box newBox;
		newBox.init(
			m_world.get(),
			glm::vec2(xPos(rng), yPos(rng)),
			glm::vec2(size(rng), size(rng)),
			m_boxTexture2D,
			ge::ColorRGBA8(color(rng), color(rng), color(rng), 255));

		m_boxes.push_back(newBox);
	}

}
