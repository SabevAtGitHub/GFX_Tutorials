#include "IMainGame.h"
#include "Timing.h"
#include "GameEngineOpenGL.h"

namespace ge
{

	IMainGame::IMainGame()
	{
	}


	IMainGame::~IMainGame()
	{
	}


	void IMainGame::runGame()
	{
		if (!init())
			return;

		FpsLimiter limiter;
		limiter.setTargetFps(DESIRED_FPS);
		m_isRunning = true;

		while (m_isRunning)
		{
			limiter.beginFrame();

			update();

			draw();

			m_fps = limiter.endFrame();
		}
	}
	

	void IMainGame::endGame()
	{
	}


	bool IMainGame::init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		initSystems();
		return false;
	}


	bool IMainGame::initSystems()
	{
		// calling SDL_Init(SDL_INIT_EVERYTHING);
		ge::init();

		// initializing sound, must happen after ge::init()
		m_audioEngine.init();

		// create the window
		m_window.create("Main Game", WINDOW_WIDTH, WINDOW_HEIGHT, ge::WINDOW_SHOWN);
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // light gray background

		// set up the camera
		m_camera.init(m_window.getWidth(), m_window.getHeight());
		m_hudCamera.init(m_window.getWidth(), m_window.getHeight());
		//m_hudCamera.setPosition(glm::vec2(m_window.getWidth() / 2, m_window.getHeight() / 2));

	    // Calling program to compile the shaders
		initShaders();


		// initializing sprite font 
		// ( must be initialized after initializing SDL, OpenGL and shaders )
		spriteFont = new ge::SpriteFont("Fonts/chintzy.ttf", 31);

		// initializing particles
		bloodParticleBatch_ = new ge::ParticleBatch2D();
		bloodParticleBatch_->init(1000, 0.05f,
			ge::ResourceManager::getTexture("Textures/particle.png"),
			[](ge::Particle2D& p, float deltaTime)
		{ p.pos += p.velocity * deltaTime;
		p.color.a = (GLubyte)(p.life * 255.f); }); // lampda for blood particles custom behavior

		particleEngine_.addParticleBatch(bloodParticleBatch_);
	}

}
