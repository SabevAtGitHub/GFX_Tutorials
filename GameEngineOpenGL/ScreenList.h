#pragma once

#include <vector>


namespace ge
{
	class MainGame;
	class IGameScreen;


	class ScreenList
	{
	public:
		ScreenList(MainGame* game);
		~ScreenList();

		void setScreen(int nextScreen);
		void addScreen(IGameScreen* newScreen);

		IGameScreen* moveNext();
		IGameScreen* mvePrevious();

		void destroy();

	protected:
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIndex = -1;
		MainGame*  m_mainGame = nullptr;
	};


}