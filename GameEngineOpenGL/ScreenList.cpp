#include "ScreenList.h"
#include "IGameScreen.h"

namespace ge
{

	ScreenList::ScreenList(IMainGame* game)
		: m_game(game)
	{
	}


	ScreenList::~ScreenList()
	{
		destroy();
	}

	void ScreenList::setScreen(int nextScreen)
	{
		m_currentScreenIndex = nextScreen;
	}

	void ScreenList::addScreen(std::unique_ptr<IGameScreen> newScreen)
	{
		m_screens.emplace_back(std::move(newScreen));
		newScreen->build();
		newScreen->setParentGame(m_game);
	}

	IGameScreen * ScreenList::moveNext()
	{
		IGameScreen* currentScreen = getCurrent();

		if (currentScreen->getNextScreenIndex() != NO_NEXT_SCREEN_INDEX) 
			m_currentScreenIndex = currentScreen->getNextScreenIndex();
		
		return getCurrent();
	}

	IGameScreen * ScreenList::movePrevious()
	{
		IGameScreen* currentScreen = getCurrent();

		if (currentScreen->getPreviousScreenIndex() != NO_NEXT_SCREEN_INDEX)
			m_currentScreenIndex = currentScreen->getPreviousScreenIndex();
		
		return getCurrent();
	}

	IGameScreen * ScreenList::getCurrent() const
	{
		if (m_currentScreenIndex == NO_NEXT_SCREEN_INDEX)
			return nullptr;

		return m_screens[m_currentScreenIndex].get();
	}

	void ScreenList::destroy()
	{
		for (auto& screen : m_screens)
		{
			screen->destroy();
			m_screens.resize(0);
			m_currentScreenIndex = NO_NEXT_SCREEN_INDEX;
		}
	}

}