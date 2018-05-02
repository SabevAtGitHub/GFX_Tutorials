#include "MainMenuScreen.h"
#include <SDL\SDL_events.h>
#include "ScreenIndices.h"


MainMenuScreen::MainMenuScreen(ge::Window* window)
	: m_window(window)
{
	m_screenIndex = SCREEN_INDEX_MAINMENU;
}


MainMenuScreen::~MainMenuScreen()
{
}

int MainMenuScreen::getNextScreenIndex() const
{
	return SCREEN_INDEX_GAMEPLAY;
}

int MainMenuScreen::getPreviousScreenIndex() const
{
	return NO_NEXT_SCREEN_INDEX;
}

void MainMenuScreen::build()
{
}

void MainMenuScreen::destroy()
{
}

void MainMenuScreen::onEntry()
{
	static const float CAMERA_SCALE = 28.f;
	// Init the camera
	m_camera.init(m_window->getWidth(), m_window->getHeight());
	m_camera.setScale(CAMERA_SCALE);

	initGUI();
}

void MainMenuScreen::onExit()
{
}

void MainMenuScreen::update()
{
	m_camera.update();
	checkInput();
}

void MainMenuScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.08f, 0.55f, 1.f); // red

	m_gui.draw();

}


void MainMenuScreen::initGUI()
{
	//ge::GUI::init("GUI");
	std::string schemeName = "AlfiskoSkin";

	m_gui.init("GUI");
	m_gui.loadScheme(schemeName);
	m_gui.setFont("DejaVuSans-10");

	std::string elementType = "Button";

#pragma region Play Game button

	auto elementPos = glm::vec4(0.45f, 0.5f, 0.1f, 0.05f);
	auto playGameButton = static_cast<CEGUI::PushButton*>(
		m_gui.createWidget(schemeName + "/" + elementType, elementPos, glm::vec4(0.0f), "Play" + elementType));
	playGameButton->setText("Play Game!");

	// Subscribe to event to be called on button click
	playGameButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&MainMenuScreen::onPlayGameClicked, this));

#pragma endregion

#pragma region Exit Game button

	elementPos = glm::vec4(0.45f, 0.65f, 0.1f, 0.05f);
	auto exitButton = static_cast<CEGUI::PushButton*>(
		m_gui.createWidget(schemeName + "/" + elementType, elementPos, glm::vec4(0.0f), "Exit" + elementType));
	exitButton->setText("Exit Game!");

	// Subscribe to event to be called on button click
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&MainMenuScreen::onExitClicked, this));

#pragma endregion

	// set custom mouse cursor
	m_gui.setMouseCursor(schemeName + "/MouseArrow");
	m_gui.showMouseCursor();
	SDL_ShowCursor(0);


}

void MainMenuScreen::checkInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		m_gui.onSDLEvent(evnt);
	}

}


bool MainMenuScreen::onExitClicked(const CEGUI::EventArgs& eargs)
{
	//std::cout << "Quitting the Game!\n";
	m_currentState = ge::ScreenState::EXIT_APPLICATION;
	return true;
}

bool MainMenuScreen::onPlayGameClicked(const CEGUI::EventArgs& eargs)
{
	//std::cout << "Quitting the Game!\n";
	m_currentState = ge::ScreenState::CHANGE_NEXT;
	return true;
}