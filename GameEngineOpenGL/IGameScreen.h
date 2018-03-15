#pragma once

namespace ge

{

	class MainGame;

	enum class ScreenState {
		NONE,
		RUNNING,
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};

	class IGameScreen
	{
	public:
		IGameScreen() {/*empty*/ }
		~IGameScreen() { /*empty*/ }

		virtual int getNextScreenIndex() const = 0;
		virtual int getPreviousScreenIndex() const = 0;

		// Called at the beginning of application
		virtual void build() = 0;
		// Called at the end of application
		virtual void destroy() = 0;

		// Called when the screen enters focus
		virtual void onEntry() = 0;
		// Called when the screen exits focus
		virtual void onExit() = 0;

		virtual void update() = 0;
		virtual void draw() = 0;

		int getIndex() const { return m_screenIndex; }

	private:
		int m_screenIndex = -1;
		ScreenState m_currentState = ScreenState::NONE;
		MainGame* m_game = nullptr;

	};

}