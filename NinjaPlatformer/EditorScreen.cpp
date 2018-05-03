#include "EditorScreen.h"
#include <SDL\SDL_events.h>
#include <GameEngineOpenGL\ResourceManager.h>


EditorScreen::EditorScreen(ge::Window* window)
	:
	m_window(window),
	m_spriteFont("Fonts/chintzy.ttf", 16)
{
	m_screenIndex = SCREEN_INDEX_EDITOR;
}

EditorScreen::~EditorScreen()
{
}

int EditorScreen::getNextScreenIndex() const
{
	return SCREEN_INDEX_NONE;
}

int EditorScreen::getPreviousScreenIndex() const
{
	return SCREEN_INDEX_MAINMENU;
}

void EditorScreen::build()
{
}

void EditorScreen::destroy()
{
}

void EditorScreen::onEntry()
{
	static const float CAMERA_SCALE = 1.0f;
	// Init the camera
	m_camera.init(m_window->getWidth(), m_window->getHeight());
	m_camera.setScale(CAMERA_SCALE);

	initGUI();
	initShaders();
	m_spriteBatch.init();
	m_blankTexture = ge::ResourceManager::getTexture("Assets/blank.png");
}

void EditorScreen::onExit()
{
	m_gui.destroy();
	m_textureProgram.dispose();
}

void EditorScreen::update()
{
	m_camera.update();
	checkInput();
}

void EditorScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.08f, 0.55f, 1.f); // red

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

	m_spriteBatch.begin();

	auto wH = (float)m_window->getHeight();
	auto wW = (float)m_window->getWidth();

	// drawing the color picker quad
	{
		const float QUAD_SIZE = 75.0f;
		auto sliderYMiddle = wH / 2.0f - (m_bSlider->getYPosition().d_scale) * wH - QUAD_SIZE;

		glm::vec4 destRect;
		// some offset for x
		destRect.x = m_bSlider->getXPosition().d_scale * m_window->getWidth() + QUAD_SIZE / 1.5f - (float)m_window->getWidth() / 2.0f;
		destRect.y = sliderYMiddle;
		destRect.z = QUAD_SIZE;
		destRect.w = QUAD_SIZE;

		auto uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		auto color = ge::ColorRGBA8((GLubyte)m_rColorVal, (GLubyte)m_gColorVal, (GLubyte)m_bColorVal, 255);

		m_spriteBatch.draw(destRect, uvRect, m_blankTexture.id, 0.0f, color);
	}

	// drawing radio button lables
	{
		auto color = ge::ColorRGBA8(255, 255, 255, 255);
		glm::vec2 pos;

		pos.x = m_rigidRadioBtn->getXPosition().d_scale * wW - wW / 2.0f + m_rigidRadioBtn->getWidth().d_offset / 2.0f;
		pos.y = wH / 2.0f - m_rigidRadioBtn->getYPosition().d_scale * wH;
		m_spriteFont.draw(m_spriteBatch, "Rigid", pos, glm::vec2(1.0f), 0.0f, color, ge::Justification::MIDDLE);

		pos.x = m_dynamicRadioBtn->getXPosition().d_scale * wW - wW / 2.0f + m_dynamicRadioBtn->getWidth().d_offset / 2.0f;
		pos.y = wH / 2.0f - m_dynamicRadioBtn->getYPosition().d_scale * wH;
		m_spriteFont.draw(m_spriteBatch, "Dynamic", pos, glm::vec2(1.0f), 0.0f, color, ge::Justification::MIDDLE);
	}

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();

	m_gui.draw();

}

void EditorScreen::initShaders()
{

	// Compile the texture
	m_textureProgram.compileShadersFromFile(std::string("Shaders/textureShading.vert"),
		std::string("Shaders/textureShading.frag"));
	m_textureProgram.addAttribute("vertexPos");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

}

void EditorScreen::initGUI()
{
	using namespace CEGUI;

	//ge::GUI::init("GUI");
	std::string scheme = "TaharezLook"; //AlfiskoSkin

	m_gui.init("GUI");
	m_gui.loadScheme(scheme);
	m_gui.setFont("DejaVuSans-10");

#pragma region Sliders
	{ // adding sliders
		std::string typ = "Slider";

		const float X_DIM = 0.02f, Y_DIM = 0.13f;
		const float X_POS = 0.08f, Y_POS = 0.25f;
		const float PADDING = 0.015f;

		auto destRect = glm::vec4(X_POS, Y_POS, X_DIM, Y_DIM);

		m_rSlider = static_cast<Slider*>(
			m_gui.createWidget(std::string(scheme + "/" + typ), destRect, glm::vec4(0.0f), "rSlider"));
		m_rSlider->setMaxValue(255.f);
		m_rSlider->setCurrentValue(m_rColorVal);
		m_rSlider->subscribeEvent(Slider::EventValueChanged,
			Event::Subscriber(&EditorScreen::onRedPickerChanged, this));
		m_rSlider->setClickStep(1.0f);

		destRect = glm::vec4(X_POS + (X_DIM + PADDING) * 1, Y_POS, X_DIM, Y_DIM);

		m_gSlider = static_cast<Slider*>(
			m_gui.createWidget(std::string(scheme + "/" + typ), destRect, glm::vec4(0.0f), "gSlider"));
		m_gSlider->setMaxValue(255.f);
		m_gSlider->setCurrentValue(m_gColorVal);
		m_gSlider->subscribeEvent(Slider::EventValueChanged,
			Event::Subscriber(&EditorScreen::onGreenPickerChanged, this));
		m_gSlider->setClickStep(1.0f);

		destRect = glm::vec4(X_POS + (X_DIM + PADDING) * 2, Y_POS, X_DIM, Y_DIM);

		m_bSlider = static_cast<Slider*>(
			m_gui.createWidget(std::string(scheme + "/" + typ), destRect, glm::vec4(0.0f), "bSlider"));
		m_bSlider->setMaxValue(255.f);
		m_bSlider->setCurrentValue(m_bColorVal);
		m_bSlider->subscribeEvent(Slider::EventValueChanged,
			Event::Subscriber(&EditorScreen::onBluePickerChanged, this));
		m_bSlider->setClickStep(1.0f);
	}
#pragma endregion

#pragma region Radio buttons
	{
		std::string typ = "RadioButton";

		const float DIM_PX = 15.0f;
		const float X_POS = 0.085f, Y_POS = 0.15f;
		const float PADDING = 0.07f;

		// rigid radio button
		auto destRectPerc = glm::vec4(X_POS, Y_POS, 0.0f, 0.0f);
		auto destRectPx = glm::vec4(0.0f, 0.0f, DIM_PX, DIM_PX);

		m_rigidRadioBtn = static_cast<RadioButton*>(m_gui.createWidget(std::string(scheme + "/" + typ), destRectPerc, destRectPx, "rigidRadioBtn"));
		m_rigidRadioBtn->subscribeEvent(RadioButton::EventMouseClick,
			Event::Subscriber(&EditorScreen::onRigidMouseClick, this));

		// dynamic radio button
		destRectPerc = glm::vec4(X_POS + PADDING, Y_POS, 0.0f, 0.0f);
		destRectPx = glm::vec4(0.0f, 0.0f, DIM_PX, DIM_PX);

		m_dynamicRadioBtn = static_cast<RadioButton*>(m_gui.createWidget(std::string(scheme + "/" + typ), destRectPerc, destRectPx, "dynamicRadioBtn"));
		m_dynamicRadioBtn->subscribeEvent(RadioButton::EventMouseClick,
			Event::Subscriber(&EditorScreen::onDynamicMouseClick, this));


		if (m_physicsMode == PhysicsMode::RIGID)
		{
			m_rigidRadioBtn->setSelected(true);
		}
		else if (m_physicsMode == PhysicsMode::DYNAMIC) {
			m_dynamicRadioBtn->setSelected(true);
		}
	}
#pragma endregion

	// set custom mouse cursor
	m_gui.setMouseCursor(std::string(scheme + "/MouseArrow"));
	m_gui.showMouseCursor();
	SDL_ShowCursor(0);

}

void EditorScreen::checkInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		m_gui.onSDLEvent(evnt);
		switch (evnt.type)
		{
		case SDL_QUIT:
			onExitClicked(CEGUI::EventArgs());
			break;
		}
	}
}

bool EditorScreen::onExitClicked(const CEGUI::EventArgs& eargs)
{
	//std::cout << "Quitting the Game!\n";
	m_currentState = ge::ScreenState::EXIT_APPLICATION;
	return true;
}

bool EditorScreen::onRedPickerChanged(const CEGUI::EventArgs& eargs) {
	m_rColorVal = m_rSlider->getCurrentValue();
	return true;
}

bool EditorScreen::onGreenPickerChanged(const CEGUI::EventArgs& eargs) {
	m_gColorVal = m_gSlider->getCurrentValue();
	return true;
}

bool EditorScreen::onBluePickerChanged(const CEGUI::EventArgs& eargs) {
	m_bColorVal = m_bSlider->getCurrentValue();
	return true;
}

bool EditorScreen::onRigidMouseClick(const CEGUI::EventArgs& eargs)
{
	m_physicsMode = PhysicsMode::RIGID;
	return true;
}

bool EditorScreen::onDynamicMouseClick(const CEGUI::EventArgs& eargs)
{
	m_physicsMode = PhysicsMode::DYNAMIC;
	return true;
}
