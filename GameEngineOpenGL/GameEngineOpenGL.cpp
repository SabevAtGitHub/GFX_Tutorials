#include <SDL2/SDL.h>

#include <GL/glew.h>
#include "GameEngineOpenGL.h"

namespace ge {
	int init() {
		// Initializing SDL
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		// This is to set the window to have 2 buffers
		// one to be drawn to while the other is being displayed at the moment
		// this keeps the screen from flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}