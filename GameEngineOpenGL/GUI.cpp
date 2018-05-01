#include "GUI.h"

namespace ge
{
	CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;

	void GUI::init(const std::string & resourceDir)
	{
		using namespace CEGUI;

		if (m_renderer == nullptr)
		{
			m_renderer = &OpenGL3Renderer::bootstrapSystem();

			auto rp = static_cast<DefaultResourceProvider*>(
				System::getSingleton().getResourceProvider());

			rp->setResourceGroupDirectory("imagesets", resourceDir + "/imagesets/");
			rp->setResourceGroupDirectory("schemes", resourceDir + "/schemes/");
			rp->setResourceGroupDirectory("fonts", resourceDir + "/fonts/");
			rp->setResourceGroupDirectory("looknfeel", resourceDir + "/looknfeel/");
			rp->setResourceGroupDirectory("layouts", resourceDir + "/layouts/");
			rp->setResourceGroupDirectory("lua_scripts", resourceDir + "/lua_scripts/");

			ImageManager::setImagesetDefaultResourceGroup("imagesets");
			Scheme::setDefaultResourceGroup("schemes");
			Font::setDefaultResourceGroup("fonts");
			WidgetLookManager::setDefaultResourceGroup("looknfeel");
			WindowManager::setDefaultResourceGroup("layouts");
			ScriptModule::setDefaultResourceGroup("lua_scripts");
		}

		m_context = &System::getSingleton().createGUIContext(
			m_renderer->getDefaultRenderTarget());
		m_root = WindowManager::getSingleton().createWindow(
			"DefaultWindow", "root");
		m_context->setRootWindow(m_root);
	}

	void GUI::destroy()
	{
		CEGUI::System::getSingleton().destroyGUIContext(*m_context);
	}

	void GUI::draw()
	{
		m_renderer->beginRendering();
		m_context->draw();
		m_renderer->endRendering();
		// TODO: bug here
		//glDisable(GL_SCISSOR_TEST);
		glEnable(GL_BLEND);
	}

	void GUI::setMouseCursor(const std::string & cursorImage)
	{
		m_context->getMouseCursor().setDefaultImage(cursorImage);
	}

	void GUI::showMouseCursor()
	{
		m_context->getMouseCursor().show();
	}

	void GUI::hideMouseCursor()
	{
		m_context->getMouseCursor().hide();
	}

	void GUI::loadScheme(const std::string & schemeName)
	{
		CEGUI::SchemeManager::getSingleton().createFromFile(schemeName + ".scheme");
	}

	void GUI::setFont(const std::string & fontFile)
	{
		CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
		m_context->setDefaultFont(fontFile);
	}

	CEGUI::Window * GUI::createWidget(const std::string& type,
		const glm::vec4& destRectPerc, const glm::vec4 destRectPix,
		const std::string& name /* ="" */)
	{
		using namespace CEGUI;
		// TODO: read the scheme name here...
		auto newWindow = WindowManager::getSingleton().createWindow(type, name);
		setWidgetDestRect(newWindow, destRectPerc, destRectPix);
		m_root->addChild(newWindow);

		return newWindow;
	}

	void GUI::setWidgetDestRect(CEGUI::Window * widget, const glm::vec4 & destRectPerc, const glm::vec4 destRectPix)
	{
		using namespace CEGUI;
		widget->setPosition(UVector2(
			UDim(destRectPerc.x, destRectPerc.x), UDim(destRectPerc.y, destRectPerc.y)));
		widget->setSize(USize(
			UDim(destRectPerc.z, destRectPerc.z), UDim(destRectPerc.w, destRectPerc.w)));
	}


}