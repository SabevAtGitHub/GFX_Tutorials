#pragma once
#include <memory>
#include "Window.h"
#include "Camera2D.h"
#include "InputManager.h"
#include "AudioManager.h"

namespace ge
{
	const float DESIRED_FPS = 60.f;
	const int MAX_TIME_STEPS = 6;
	const float MAX_DELTA_TIME = 1;
	const float MILLISEC_PER_SEC = 1000.f;
	const float CAMERA_SCALE = 1.f / 2.5f;
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	class ScreenList;
	class IGameScreen;

	class IMainGame
	{
	public:
		IMainGame();
		virtual ~IMainGame();

		void runGame();
		void endGame();

		virtual void onInit() = 0;
		virtual void addScreens() = 0;
		virtual void onExit() = 0;

		const float getFps() const { return m_fps; }
	protected:
		bool init();
		bool initSystems();

	protected:
		std::unique_ptr<ScreenList> m_screenList = nullptr;
		IGameScreen* m_currentScreen = nullptr;
		Window m_window;
		Camera2D m_camera;
		Camera2D m_hudCamera;
		InputManager m_inputMngr;
		AudioManager m_audioEngine;

		bool m_isRunning = false;
		float m_fps = 0.f;

	};
}

