#pragma once
#include<GL\glew.h>
#include "GLTexture.h"
#include <string>

namespace ge {
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float xPos, float yPos, float width, float height, std::string texturePath);
		void draw();

	private:
		float x, y, w, h;
		GLuint m_vboId; // 32 bit unsigned int
		GLTexture m_texture;

	};
}

