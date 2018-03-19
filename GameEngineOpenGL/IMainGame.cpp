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


	void IMainGame::run()
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


	void IMainGame::exit()
	{
		m_currentScreen->onExit();

		if (m_screenList) {
			m_screenList->destroy();
			m_screenList.reset();
		}

		m_isRunning = false;
	}


	bool IMainGame::init()
	{
		// calling SDL_Init(SDL_INIT_EVERYTHING);
		if (!initSystems())
			return false;

		// apply custom init logic here ( for example,
		// to overwrite any window settings etc.)
		onInit();
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
