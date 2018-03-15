#include "IMainGame.h"
#include "Timing.h"
#include "GameEngineOpenGL.h"
#include "ScreenList.h"
#include "IGameScreen.h"

namespace ge
{

	IMainGame::IMainGame()
	{
	}


	IMainGame::~IMainGame()
	{
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
	

	void IMainGame::endGame()
	{
	}


	bool IMainGame::init()
	{
		initSystems();
		onInit();

		return true;
	}


	bool IMainGame::initSystems()
	{
		// calling SDL_Init(SDL_INIT_EVERYTHING);
		ge::init();

		// create the window
		m_window.create("Main Game", WINDOW_WIDTH, WINDOW_HEIGHT, ge::WINDOW_SHOWN);
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // light gray background

		// set up the camera
		m_camera.init(m_window.getWidth(), m_window.getHeight());

		return true;
	}

}
