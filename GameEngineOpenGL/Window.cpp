#include "Window.h"
#include "ErrManager.h"


namespace ge {

	Window::Window() { /* empty */ }

	Window::~Window() { /* empty */ }

	int Window::create(std::string windowName, float scrWidth, float scrHeight, WindowFlags windowFlags)
	{
		w = scrWidth;
		h = scrHeight;

		// assigning all flags (if any)
		Uint32 flags = SDL_WINDOW_OPENGL; /// default window flag
		if (windowFlags & WINDOW_SHOWN) {
			flags |= SDL_WINDOW_SHOWN;
		}
		if (windowFlags & WINDOW_HIDDEN) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (windowFlags & WINDOW_FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (windowFlags & WINDOW_BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		/// Creating the main window
		sdlWindow = SDL_CreateWindow(windowName.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			(int)w,
			(int)h,
			flags);
		if (nullptr == sdlWindow) {
			fatalError("MainGame: SDL Window could not be created!");
		}

		// Creating SDL Context and sending it to the main window
		SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
		if (nullptr == glContext) {
			fatalError("MainGame: SDL_GL context could not be created!");
		}

		// Initializing Glue (does hardware check etc.)
		GLenum err = glewInit();
		if (GLEW_OK != err) {
			fatalError("MainGame: Could not initialize Glew!");
		}

		std::printf("***	OpenGL version %s	***\n", glGetString(GL_VERSION));

		// Setting the screen background color
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		// setting VSYNC, 0 = off, 1 = on
		SDL_GL_SetSwapInterval(0);

		// enable transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer()
	{
		// Flushing current screen
		SDL_GL_SwapWindow(sdlWindow);
	}

}