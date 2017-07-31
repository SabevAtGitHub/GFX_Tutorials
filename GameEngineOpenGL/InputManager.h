#pragma once
#include <unordered_map>
#include <glm\glm.hpp>

namespace ge {
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void pressKey(unsigned int keyId);
		void releaseKey(unsigned int keyId);

		/// <summary>
		/// True if the key was pressed during last frame
		/// </summary>
		bool isKeyDown(unsigned int keyId);

		/// <summary>
		/// True if the key is held down
		/// </summary>		
		bool isKeyPressed(unsigned int keyId);

		void setMouseCoords(float x, float y);

		// Getters
		glm::vec2 getMouseCoords() const { return mouseCoords; }

	private:
		bool wasKeyDown(unsigned int keyId);
		std::unordered_map<unsigned int, bool> keyMap;
		std::unordered_map<unsigned int, bool> prevKeyMap;
		glm::vec2 mouseCoords;
	};
}

