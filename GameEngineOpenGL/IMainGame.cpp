#include "IMainGame.h"
#include "Timing.h"
#include "GameEngineOpenGL.h"

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

		// initializing sound, must happen after ge::init()
		m_audioEngine.init();

		// create the window
		m_window.create("Main Game", WINDOW_WIDTH, WINDOW_HEIGHT, ge::WINDOW_SHOWN);
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // light gray background

		// set up the camera
		m_camera.init(m_window.getWidth(), m_window.getHeight());

	    // Calling program to compile the shaders
		initShaders();

		return true;
	}

	void IMainGame::initShaders()
	{
		// adding attributes for each variable in the shader files
		// right now the entry point is the .vert file
		m_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
		m_colorProgram.addAttribute("vertexPos");
		m_colorProgram.addAttribute("vertexColor");
		m_colorProgram.addAttribute("vertexUV");

		// linking the2 shader files
		m_colorProgram.linkShaders();
	}

}
