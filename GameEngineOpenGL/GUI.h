#pragma once
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include <string>
#include <memory>
#include <glm\glm.hpp>

namespace ge
{
	class GUI
	{
	public:

		void init(const std::string&  resourceDir);
		void destroy();
		void draw();

		void loadScheme(const std::string& schemeName);
		void setFont(const std::string& fontFile);

		// Loolup the desired type and use static_cast to convert the Window* to it
		CEGUI::Window* createWidget(const std::string& type,
			const glm::vec4& destRectPerc, 
			const glm::vec4 destRectPix,
			const std::string& name = "");
		static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer; }
		const CEGUI::GUIContext* getContext() { return m_context; }

		static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4 destRectPix);
	private:
		static CEGUI::OpenGL3Renderer* m_renderer;
		CEGUI::GUIContext* m_context = nullptr;
		CEGUI::Scheme* m_scheme = nullptr;
		CEGUI::Window* m_root = nullptr;
	};
}