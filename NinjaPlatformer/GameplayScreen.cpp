#include "GameplayScreen.h"
#include <GameEngineOpenGL\ResourceManager.h>


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

	initShaders();

	// load the texture
	m_texture2D = ge::ResourceManager::getTexture("Assets/bricks_top.png");

	// Init the camera
	m_camera.init(m_window->getWidth(), m_window->getHeight());
	m_camera.setScale(32.f);
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


	m_textureProgram.use();


	// this specifies which texture were binding, since multiple
	// textures can be used at the same time in the shaders
	GLint textureLocation = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);

	// grab the camera matrix
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();

	// drawing / adjusting the camera here - todo - may change
	// set the camera matrix and pass it ot shader
	GLint pLocation = m_textureProgram.getUniformLocation("P");

	// uploading the camera matrix to the GPU
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//// Upload texture uniform
	//auto textureUniform = m_textureProgram.getUniformLocation(pLocation);

	m_spriteBatch.begin();

	// draw all boxes
	for (auto& b : m_boxes) {
		glm::vec4 destRect;
		destRect.x = b.getBody()->GetPosition().x;
		destRect.y = b.getBody()->GetPosition().y;
		destRect.z = b.getDimentions().x;
		destRect.w = b.getDimentions().y;

		m_spriteBatch.draw(destRect, glm::vec4(0.f, 0.f, 1.f, 1.f), m_texture2D.id, 0.f, ge::ColorRGBA8(255,0, 0, 255), b.getBody()->GetAngle());
	}

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();

}

void GameplayScreen::checkInput()
{
}

void GameplayScreen::initShaders()
{
	// adding attributes for each variable in the shader files
	// right now the entry point is the .vert file
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPos");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");

	// linking the 2 shader files
	m_textureProgram.linkShaders();
}