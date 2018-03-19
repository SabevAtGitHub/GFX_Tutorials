#include "IMainGame.h"
#include "Timing.h"
#include "GameEngineOpenGL.h"
#include "ScreenList.h"
#include "IGameScreen.h"

namespace ge
{

	IMainGame::IMainGame()
	{/*empty*/
	}


	IMainGame::~IMainGame()
	{/*empty*/
	}


	void IMainGame::runGame()
	{
		if (!init())
			return;

		FpsLimiter limiter;
		limiter.setTargetFps(DESIRED_FPS);

		m_isRunning = true;
		while (m_isRunning)
		{
			limiter.beginFrame();
			update();
			draw();
			m_fps = limiter.endFrame();
		}
	}


	void IMainGame::exitGame()
	{
		m_currentScreen->onExit();

		if (m_screenList) {
			m_screenList->destroy();
			m_screenList.reset();
		}

		m_isRunning = false;
	}


	void IMainGame::update()
	{
		if (m_currentScreen) {
			switch (m_currentScreen->getState())
			{
			case ge::ScreenState::NONE:
				break;
			case ge::ScreenState::RUNNING:
				m_currentScreen->update();
				break;
			case ge::ScreenState::EXIT_APPLICATION:
				exitGame();
				break;
			case ge::ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
				if (m_currentScreen != nullptr) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ge::ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrevious();
				if (m_currentScreen != nullptr) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			default:
				break;
			}
		}
		else
		{
			exitGame();
		}
	}

	void IMainGame::draw()
	{
		if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING) {
			m_currentScreen->draw();
		}
	}

	bool IMainGame::init()
	{
		// calling SDL_Init(SDL_INIT_EVERYTHING);
		if (!initSystems())
			return false;

		// apply custom init logic here ( for example,
		// to overwrite any window settings etc.)
		onInit();
		addScreens();

		m_currentScreen = m_screenList->getCurrent();
		m_currentScreen->onEntry();
		return true;
	}


	bool IMainGame::initSystems()
	{
		// initiating the game engine
		ge::init();

		// create the window
		m_window.create("Main Game", WINDOW_WIDTH, WINDOW_HEIGHT, ge::WINDOW_SHOWN);
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // light gray background

		return true;
	}

}
