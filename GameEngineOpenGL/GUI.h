#pragma once
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <string>
#include <memory>
#include <SDL2/SDL_events.h>
#include <glm/glm.hpp>

namespace ge
{GUI
	class GUI
	{
	public:
		static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer; }
		static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4 destRectPix);

	public:
		void init(const std::string&  resourceDir);
		void destroy();
		void draw();
		void update();
		void setMouseCursor(const std::string& cursorImage);
		void showMouseCursor();
		void hideMouseCursor();
		void onSDLEvent(SDL_Event & evnt);
		void loadScheme(const std::string& schemeName);
		void setFont(const std::string& fontFile);
		
		// Creates widget and is adding it as a child to the root window
		// Use static_cast to convert the Window* to the desired type
		CEGUI::Window* createWidget(const std::string& type,
			const glm::vec4& destRectPerc,
			const glm::vec4 destRectPix,
			const std::string& name = "");

		// Creates widget and is adding it as a child to the root window
		// Use static_cast to convert the Window* to the desired type
		CEGUI::Window* createWidget(CEGUI::Window* parent, const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name = "");
		
		const CEGUI::GUIContext* getContext() { return m_context; }

	private:
		static CEGUI::OpenGL3Renderer* m_renderer;

	private:
		CEGUI::GUIContext* m_context = nullptr;
		CEGUI::Scheme* m_scheme = nullptr;
		CEGUI::Window* m_root = nullptr;
		unsigned int m_lastTime = 0;
	};
}