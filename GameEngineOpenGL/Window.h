#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <string>

namespace ge {
	enum WindowFlags {
		WINDOW_SHOWN = 0x01,
		WINDOW_HIDDEN = 0x02,
		WINDOW_FULLSCREEN = 0x04,
		WINDOW_BORDERLESS = 0x08,

	};

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, float scrWidth, float scrHeight, WindowFlags windowFlags = WINDOW_SHOWN);
		void swapBuffer();

		float getScrWidth() { return w; }
		float getScrHeight() { return h; }

	private:
		SDL_Window* sdlWindow;
		float w, h;
	};
}

