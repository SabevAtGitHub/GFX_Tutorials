#pragma once

#define NO_NEXT_SCREEN_INDEX -1

namespace ge
{
	class IMainGame;

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
		friend class ScreenList;

	public:
		IGameScreen() {/*empty*/ }
		virtual ~IGameScreen() { /*empty*/ }

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

		int getScreenIndex() const { return m_screenIndex; }

		void setRunning() { m_currentState = ScreenState::RUNNING; }

		ScreenState getState() const { return m_currentState; }

		void setParentGame(IMainGame* game) { m_game = game; }

	protected:
		int m_screenIndex = -1;
		ScreenState m_currentState = ScreenState::NONE;
		IMainGame* m_game = nullptr;
	};

}