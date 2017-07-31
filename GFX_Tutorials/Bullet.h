#pragma once
#include <glm\glm.hpp>
#include <GameEngineOpenGL\SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction,
		float velocity, int lifeTime);
	~Bullet();

	void draw(ge::SpriteBatch& spriteBatch);
	/// <summary>
	/// Returning true if bulet's life = 0
	/// </summary>
	/// <returns>True if bullet must be destroyed</returns>
	bool update();

private:
	int lifeT;
	float speed;
	glm::vec2 dir;
	glm::vec2 pos;
};

