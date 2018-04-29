#pragma once
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include <memory>

namespace ge
{

	class GUI
	{
	public:
		
		static void init(const std::string&  resourceDir);
		void destroy();
		void draw();

		void loadScheme(const std::string& schemeFile);
		void setFont(const std::string& fontFile);

		static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer.get(); }
		const CEGUI::GUIContext* getContext() { return m_context; }

	private:
		static std::unique_ptr<CEGUI::OpenGL3Renderer> m_renderer;
		CEGUI::GUIContext* m_context = nullptr;
	};


}