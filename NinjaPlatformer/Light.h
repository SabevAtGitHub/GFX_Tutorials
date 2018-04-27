#pragma once
#include <GameEngineOpenGL\Vertex.h>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <glm\glm.hpp>

class Light
{
public:
	void draw(ge::SpriteBatch& spriteBatch) {
		glm::vec4 destRect;
		auto uvRect = glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f);

		destRect.x = pos.x - size / 2.f;
		destRect.y = pos.y - size / 2.f;
		destRect.z = size;
		destRect.w = size;

		spriteBatch.draw(destRect, uvRect, 0, 0.f, color, 0);
	}

		
public:
	ge::ColorRGBA8 color;
	glm::vec2 pos;
	float size;

};
