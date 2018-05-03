#include "GUI.h"
#include <SDL\SDL_timer.h>

namespace ge
{
#pragma region Static Init
	
	CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;

#pragma endregion

#pragma region Helpers

	CEGUI::Key::Scan SDLKeyToCEGUIKey(SDL_Keycode key)
	{
		using namespace CEGUI;
		switch (key)
		{
		case SDLK_BACKSPACE:    return Key::Backspace;
		case SDLK_TAB:          return Key::Tab;
		case SDLK_RETURN:       return Key::Return;
		case SDLK_PAUSE:        return Key::Pause;
		case SDLK_ESCAPE:       return Key::Escape;
		case SDLK_SPACE:        return Key::Space;
		case SDLK_COMMA:        return Key::Comma;
		case SDLK_MINUS:        return Key::Minus;
		case SDLK_PERIOD:       return Key::Period;
		case SDLK_SLASH:        return Key::Slash;
		case SDLK_0:            return Key::Zero;
		case SDLK_1:            return Key::One;
		case SDLK_2:            return Key::Two;
		case SDLK_3:            return Key::Three;
		case SDLK_4:            return Key::Four;
		case SDLK_5:            return Key::Five;
		case SDLK_6:            return Key::Six;
		case SDLK_7:            return Key::Seven;
		case SDLK_8:            return Key::Eight;
		case SDLK_9:            return Key::Nine;
		case SDLK_COLON:        return Key::Colon;
		case SDLK_SEMICOLON:    return Key::Semicolon;
		case SDLK_EQUALS:       return Key::Equals;
		case SDLK_LEFTBRACKET:  return Key::LeftBracket;
		case SDLK_BACKSLASH:    return Key::Backslash;
		case SDLK_RIGHTBRACKET: return Key::RightBracket;
		case SDLK_a:            return Key::A;
		case SDLK_b:            return Key::B;
		case SDLK_c:            return Key::C;
		case SDLK_d:            return Key::D;
		case SDLK_e:            return Key::E;
		case SDLK_f:            return Key::F;
		case SDLK_g:            return Key::G;
		case SDLK_h:            return Key::H;
		case SDLK_i:            return Key::I;
		case SDLK_j:            return Key::J;
		case SDLK_k:            return Key::K;
		case SDLK_l:            return Key::L;
		case SDLK_m:            return Key::M;
		case SDLK_n:            return Key::N;
		case SDLK_o:            return Key::O;
		case SDLK_p:            return Key::P;
		case SDLK_q:            return Key::Q;
		case SDLK_r:            return Key::R;
		case SDLK_s:            return Key::S;
		case SDLK_t:            return Key::T;
		case SDLK_u:            return Key::U;
		case SDLK_v:            return Key::V;
		case SDLK_w:            return Key::W;
		case SDLK_x:            return Key::X;
		case SDLK_y:            return Key::Y;
		case SDLK_z:            return Key::Z;
		case SDLK_DELETE:       return Key::Delete;
		case SDLK_KP_PERIOD:    return Key::Decimal;
		case SDLK_KP_DIVIDE:    return Key::Divide;
		case SDLK_KP_MULTIPLY:  return Key::Multiply;
		case SDLK_KP_MINUS:     return Key::Subtract;
		case SDLK_KP_PLUS:      return Key::Add;
		case SDLK_KP_ENTER:     return Key::NumpadEnter;
		case SDLK_KP_EQUALS:    return Key::NumpadEquals;
		case SDLK_UP:           return Key::ArrowUp;
		case SDLK_DOWN:         return Key::ArrowDown;
		case SDLK_RIGHT:        return Key::ArrowRight;
		case SDLK_LEFT:         return Key::ArrowLeft;
		case SDLK_INSERT:       return Key::Insert;
		case SDLK_HOME:         return Key::Home;
		case SDLK_END:          return Key::End;
		case SDLK_PAGEUP:       return Key::PageUp;
		case SDLK_PAGEDOWN:     return Key::PageDown;
		case SDLK_F1:           return Key::F1;
		case SDLK_F2:           return Key::F2;
		case SDLK_F3:           return Key::F3;
		case SDLK_F4:           return Key::F4;
		case SDLK_F5:           return Key::F5;
		case SDLK_F6:           return Key::F6;
		case SDLK_F7:           return Key::F7;
		case SDLK_F8:           return Key::F8;
		case SDLK_F9:           return Key::F9;
		case SDLK_F10:          return Key::F10;
		case SDLK_F11:          return Key::F11;
		case SDLK_F12:          return Key::F12;
		case SDLK_F13:          return Key::F13;
		case SDLK_F14:          return Key::F14;
		case SDLK_F15:          return Key::F15;
		case SDLK_RSHIFT:       return Key::RightShift;
		case SDLK_LSHIFT:       return Key::LeftShift;
		case SDLK_RCTRL:        return Key::RightControl;
		case SDLK_LCTRL:        return Key::LeftControl;
		case SDLK_RALT:         return Key::RightAlt;
		case SDLK_LALT:         return Key::LeftAlt;
		case SDLK_SYSREQ:       return Key::SysRq;
		case SDLK_MENU:         return Key::AppMenu;
		case SDLK_POWER:        return Key::Power;
		default:                return Key::Unknown;
		}
	}

	CEGUI::MouseButton SDLMouseBtnToCEGUIMouseBtn(Uint8 sdlButton)
	{
		switch (sdlButton)
		{
		case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;

		case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;

		case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;

		case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;

		case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;

		default: return CEGUI::MouseButton::NoButton;
		}
	}

	void GUI::setWidgetDestRect(CEGUI::Window * widget, const glm::vec4 & destRectPerc, const glm::vec4 destRectPix)
	{
		using namespace CEGUI;
		widget->setPosition(UVector2(
			UDim(destRectPerc.x, destRectPix.x), UDim(destRectPerc.y, destRectPix.y)));
		widget->setSize(USize(
			UDim(destRectPerc.z, destRectPix.z), UDim(destRectPerc.w, destRectPix.w)));
	}

#pragma endregion

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
		glEnable(GL_BLEND);
	}

	void GUI::update()
	{
		unsigned int elapsedTime;
		if (!m_lastTime)
		{
			elapsedTime = 0;
			m_lastTime = SDL_GetTicks();
		}
		else {
			unsigned int nextTime = SDL_GetTicks();
			elapsedTime = nextTime - m_lastTime;
			m_lastTime = nextTime;
		}

		m_context->injectTimePulse((float)elapsedTime / 1000.0f);
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

	void GUI::onSDLEvent(SDL_Event & evnt)
	{
		CEGUI::utf32 codePoint;

		switch (evnt.type)
		{
		case SDL_MOUSEMOTION:
			m_context->injectMousePosition(
				(float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_context->injectKeyDown(
				SDLKeyToCEGUIKey(evnt.key.keysym.sym));
			break;
		case SDL_KEYUP:
			m_context->injectKeyUp(
				SDLKeyToCEGUIKey(evnt.key.keysym.sym));
			break;
		case SDL_TEXTINPUT:
			codePoint = 0;
			for (int i = 0; evnt.text.text[i] != '\0'; i++)
			{
				codePoint |= ((CEGUI::utf32)evnt.text.text[i] << (i * 8));
				m_context->injectChar(codePoint);
			}

			//m_context->
		case SDL_MOUSEBUTTONDOWN:
			m_context->injectMouseButtonDown(
				SDLMouseBtnToCEGUIMouseBtn(evnt.button.button));
			break;
		case SDL_MOUSEBUTTONUP:
			m_context->injectMouseButtonUp(
				SDLMouseBtnToCEGUIMouseBtn(evnt.button.button));
			break;
		}
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
		return createWidget(m_root, type, destRectPerc, destRectPix, name);
	}

	CEGUI::Window * GUI::createWidget(CEGUI::Window * parent, const std::string & type, const glm::vec4 & destRectPerc, const glm::vec4 & destRectPix, const std::string & name)
	{
		using namespace CEGUI;
		// TODO: read the scheme name here...
		auto newWindow = WindowManager::getSingleton().createWindow(type, name);
		setWidgetDestRect(newWindow, destRectPerc, destRectPix);
		parent->addChild(newWindow);

		return newWindow;
	}

}