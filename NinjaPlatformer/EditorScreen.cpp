#include "EditorScreen.h"
#include <SDL\SDL_events.h>


EditorScreen::EditorScreen(ge::Window* window)
	: m_window(window)
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
}

void EditorScreen::onExit()
{
	m_gui.destroy();
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

	m_gui.draw();

}

void EditorScreen::initGUI()
{
	//ge::GUI::init("GUI");
	std::string schemeName = "TaharezLook";

	m_gui.init("GUI");
	m_gui.loadScheme(schemeName);
	m_gui.setFont("DejaVuSans-10");

	std::string elementType = "Slider";


#pragma region ColorPicker slider

	auto elementPos = glm::vec4(0.2f, 0.20f, 0.02f, 0.2f);
	std::string elementDescrib = { schemeName + "/" + elementType };

	auto slider1 = static_cast<CEGUI::Slider*>(
		m_gui.createWidget(elementDescrib, elementPos, glm::vec4(0.0f), "Slider1"));
	slider1->setMaxValue(255.f);
	slider1->setCurrentValue(m_colorPickerRed);


#pragma endregion

	// set custom mouse cursor
	m_gui.setMouseCursor(std::string(schemeName + "/MouseArrow"));
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