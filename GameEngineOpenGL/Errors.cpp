#include "ErrManager.h"
#include <iostream>
#include <SDL\SDL.h>
#include <cstdlib>

namespace ge {
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Press any key to exit...";

		int tmp;
		std::cin >> tmp;

		SDL_Quit();
		exit(69);
	}
}
