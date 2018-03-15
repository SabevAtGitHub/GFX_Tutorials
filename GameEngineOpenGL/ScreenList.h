#pragma once

#include <vector>
#include <memory>

namespace ge
{
	class IMainGame;
	class IGameScreen;


	class ScreenList
	{
	public:
		ScreenList(IMainGame* game);
		~ScreenList();

		void setScreen(int nextScreen);
		void addScreen(std::unique_ptr<IGameScreen> newScreen);

		IGameScreen* moveNext();
		IGameScreen* movePrevious();
		IGameScreen* getCurrent() const;

		void destroy();

	protected:
		std::vector<std::unique_ptr<IGameScreen>> m_screens;
		int m_currentScreenIndex = -1;
		IMainGame*  m_game = nullptr;
	};


}