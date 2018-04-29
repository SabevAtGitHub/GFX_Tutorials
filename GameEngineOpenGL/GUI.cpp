#include "GUI.h"

namespace ge
{
	std::unique_ptr<CEGUI::OpenGL3Renderer> GUI::m_renderer = nullptr;

	void GUI::init(const std::string & resourceDir)
	{
		using namespace CEGUI;

		if (m_renderer == nullptr)
		{
			m_renderer = std::make_unique<OpenGL3Renderer>(&OpenGL3Renderer::bootstrapSystem());

			auto rp = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());

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
	}

	void GUI::destroy()
	{
	}

	void GUI::draw()
	{
	}

	void GUI::loadScheme(const std::string & schemeFile)
	{
		CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
	}

	void GUI::setFont(const std::string & fontFile)
	{
		m_context->setDefaultFont(fontFile);
	}


}