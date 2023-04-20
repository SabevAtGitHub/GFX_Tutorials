#include "ErrManager.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <cstdlib>

namespace ge {

	void GLClearError() {
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLLogCall(const char* function, const char* file, int line) {
		if (GLenum error = glGetError()) {
			std::cout << "[OpenGL Error] (0x0" << std::hex << error << std::dec << "): " <<
				function << ", File: " << file << ", Line: " << line << std::endl;
			return false;
		}
		return true;
	}

	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Press any key to exit...";

		int tmp;
		std::cin >> tmp;

		SDL_Quit();
		exit(69);
	}
}
