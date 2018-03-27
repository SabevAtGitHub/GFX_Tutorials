#pragma once
#include <memory>
#include "Window.h"
#include "InputManager.h"


namespace ge
{
	const float DESIRED_FPS = 60.f;
	const int MAX_TIME_STEPS = 6;
	const float MAX_DELTA_TIME = 1;
	const float MILLISEC_PER_SEC = 1000.f;
	const float CAMERA_SCALE = 1.f / 2.5f;
	const int WINDOW_WIDTH = 1366;
	const int WINDOW_HEIGHT = 768;

	class ScreenList;
	class IGameScreen;

	class IMainGame
	{
	public:
		IMainGame();
		virtual ~IMainGame();

		void runGame();
		void exitGame();

		// Implement any custom init logic here ( for example,
		// to overwrite any window settings etc.)
		virtual void onInit() = 0;

		virtual void addScreens() = 0;

		// Implement any custom exit logic here
		virtual void onExit() = 0;

		void update();
		void draw();

		const float getFps() const { return m_fps; }

		void onSDLEvent(SDL_Event& evnt);

	protected:
		bool init();
		bool initSystems();
		

	protected:
		std::unique_ptr<ScreenList> m_screenList = nullptr;
		IGameScreen* m_currentScreen = nullptr;
		Window m_window;
		InputManager m_inputManager;

		bool m_isRunning = false;
		float m_fps = 0.f;

	};
}

