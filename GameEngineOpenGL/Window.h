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

		int create(std::string windowTitle, int width, int height, WindowFlags windowFlags = WINDOW_SHOWN);
		void swapBuffer();

		const float getWidth() const { return m_width; }
		const float getHeight() const { return m_height; }

	private:
		SDL_Window* sdlWindow;
		int m_width = 600;
		int m_height = 480;
	};
}

