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
	initGraphics();

	initActors();

	initLights();

	initGUI();
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

	// Rendering the boxes and the player
#pragma region Rendering the boxes and the player

	m_spriteBatch.begin();

	// draw all boxes
	for (auto& b : m_boxes) {
		b.draw(m_spriteBatch);
	}

	m_player.draw(m_spriteBatch);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();
#pragma endregion // Rendering the boxes and the player

	// Debug rendering...
#pragma region Debug rendering...

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
#pragma endregion // Debug rendering...

	// Render test lights
#pragma region Render test lights

	m_lightProgram.use();
	pLocation = m_lightProgram.getUniformLocation("P");
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	// additive blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_spriteBatch.begin();

	// drawing player light
	m_playerLight.pos = m_player.getPos();
	m_playerLight.draw(m_spriteBatch);

	// drawing mouse light
	m_mouseLight.pos = m_camera.covertScreenToWorld(m_game->inputManager.getMouseCoords());
	m_mouseLight.draw(m_spriteBatch);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_lightProgram.unuse();

	// reset blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#pragma endregion // Render test lights

	m_gui.draw();
	
}

void GameplayScreen::initGUI()
{	
	//ge::GUI::init("GUI");
	std::string schemeName = "AlfiskoSkin";

	m_gui.init("GUI");
	m_gui.loadScheme(schemeName);
	m_gui.setFont("DejaVuSans-10");

#pragma region TODO: TEST GUI ELEMENTS

	std::string elementName;
	elementName = "Button";

	auto testButton = static_cast<CEGUI::PushButton*>(
		m_gui.createWidget(schemeName + "/" + elementName, glm::vec4(0.5f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "Test" + elementName));
	testButton->setText("Hello World!");

	elementName = "Editbox";

	auto testCombobox = static_cast<CEGUI::Editbox*>(
		m_gui.createWidget(schemeName + "/" + elementName, glm::vec4(0.2f, 0.2f, 0.1f, 0.05f), glm::vec4(0.0f), "Test" + elementName));

	m_gui.setMouseCursor(schemeName + "/MouseArrow");
	m_gui.showMouseCursor();
	SDL_ShowCursor(0);

#pragma endregion
}

void GameplayScreen::initGraphics()
{
	// Initialize spriteBatch
	m_spriteBatch.init();

	initShaders();
}

void GameplayScreen::initShaders()
{
	// Compile the texture
	m_textureProgram.compileShadersFromFile(std::string("Shaders/textureShading.vert"),
		std::string("Shaders/textureShading.frag"));
	m_textureProgram.addAttribute("vertexPos");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	// compile the lights
	m_lightProgram.compileShadersFromFile(std::string("Shaders/lightShading.vert"),
		std::string("Shaders/lightShading.frag"));
	m_lightProgram.addAttribute("vertexPos");
	m_lightProgram.addAttribute("vertexColor");
	m_lightProgram.addAttribute("vertexUV");
	m_lightProgram.linkShaders();
}

void GameplayScreen::initLights()
{
	// init the lights
	m_playerLight.color = ge::ColorRGBA8(255, 255, 255, 128);
	m_playerLight.pos = m_player.getPos();
	m_playerLight.size = 20;

	// init the lights
	m_mouseLight.color = ge::ColorRGBA8(230, 20, 255, 128);
	m_mouseLight.pos = m_player.getPos();
	m_mouseLight.size = 35;
}

void GameplayScreen::initActors()
{

	static const float CAMERA_SCALE = 28.f;
	static const float WORLD_FLOOR_HEIGHT = -20.f;
	static const float GRAVITY_RATE = -24.f;
	static const int NUM_BOXES = 47;
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

void GameplayScreen::checkInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
		m_gui.onSDLEvent(evnt);
	}
}
// creating number of boxes