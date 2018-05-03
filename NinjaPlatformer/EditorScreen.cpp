#include "EditorScreen.h"
#include <SDL\SDL_events.h>
#include <GameEngineOpenGL\ResourceManager.h>


EditorScreen::EditorScreen(ge::Window* window)
	:
	m_window(window),
	m_spriteFont("Fonts/Xcelsion.ttf", 32)
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
	static const float CAMERA_SCALE = 28.f;
	// Init the camera
	m_camera.init(m_window->getWidth(), m_window->getHeight());
	m_camera.setScale(CAMERA_SCALE);

	initGUI();
	m_spriteBatch.init();
	
	initShaders();

	ge::ResourceManager::getTexture("Assets/blank.png");

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

	{// drawing the color picker quad
		m_spriteBatch.begin();

		glm::vec4 destRect;
		destRect.x = m_bSlider->getPosition().d_x.px + 20.0f; // some offset
		destRect.y = m_bSlider->getPosition().d_y.px;
		destRect.z = 50.0f;
		destRect.w = 50.0f;

		auto color = ge::ColorRGBA8((GLubyte)m_rColorVal, (GLubyte)m_gColorVal, (GLubyte)m_bColorVal, 255);

		m_spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1 , 0.0f, color);

		m_spriteBatch.end();
	}

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
		const float X_DIM = 0.017f, Y_DIM = 0.13f;
		const float X_POS = 0.08f, Y_POS = 0.1f;
		const float PADDING = 0.022f;

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

bool EditorScreen::onRedPickerChanged() {
	m_rColorVal = m_rSlider->getCurrentValue();
	return true;
}

bool EditorScreen::onGreenPickerChanged() {
	m_gColorVal = m_gSlider->getCurrentValue();
	return true;
}

bool EditorScreen::onBluePickerChanged() {
	m_bColorVal = m_bSlider->getCurrentValue();
	return true;
}