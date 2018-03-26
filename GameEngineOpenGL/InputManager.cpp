#include "InputManager.h"
#include "InputManager.h"



namespace ge {
	InputManager::InputManager() : m_mouseCoords(0.f) { /* empty */ }
		InputManager::~InputManager() { /* empty */ }

	/// <summary>
	/// copies new key map over the old key map
	/// </summary>
	void InputManager::update()
	{
		for each (auto& key in m_keyMap) {
			m_prevKeyMap[key.first] = key.second;
		}
	}

	void InputManager::pressKey(unsigned int keyId)
	{
		m_keyMap[keyId] = true; // adding the keyId if not found
	}

	void InputManager::releaseKey(unsigned int keyId)
	{
		m_keyMap[keyId] = false;
	}

	bool InputManager::isKeyDown(unsigned int keyId)
	{
		auto it = m_keyMap.find(keyId);
		if (it != m_keyMap.end()) {
			return it->second;
		}
		return false;
	}

	/// <summary>
	/// True if a key was pressed this frame
	/// </summary>
	bool InputManager::isKeyPressed(unsigned int keyId)
	{
		if (isKeyDown(keyId) == true && wasKeyDown(keyId) == false) {
			return true;
		}
		return false;
	}

	// private
	bool InputManager::wasKeyDown(unsigned int keyId)
	{
		auto it = m_prevKeyMap.find(keyId);
		if (it != m_prevKeyMap.end()) {
			return it->second;
		}
		return false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

}