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
	std::string schemeName = "AlfiskoSkin";

	m_gui.init("GUI");
	m_gui.loadScheme(schemeName);
	m_gui.setFont("DejaVuSans-10");

	std::string elementType = "Button";

#pragma region Exit Game button

	auto elementPos = glm::vec4(0.45f, 0.57f, 0.1f, 0.05f);
	auto exitButton = static_cast<CEGUI::PushButton*>(
		m_gui.createWidget(schemeName + "/" + elementType, elementPos, glm::vec4(0.0f), "ExitButton"));
	exitButton->setText("Exit Game!");

	// Subscribe to event to be called on button click
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&EditorScreen::onExitClicked, this));

#pragma endregion

	// set custom mouse cursor
	m_gui.setMouseCursor(schemeName + "/MouseArrow");
	m_gui.showMouseCursor();
	SDL_ShowCursor(0);

}


void EditorScreen::checkInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		m_gui.onSDLEvent(evnt);
	}
}


bool EditorScreen::onExitClicked(const CEGUI::EventArgs& eargs)
{
	//std::cout << "Quitting the Game!\n";
	m_currentState = ge::ScreenState::EXIT_APPLICATION;
	return true;
}