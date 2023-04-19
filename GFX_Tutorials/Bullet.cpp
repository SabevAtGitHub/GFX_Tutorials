#include "Bullet.h"
#include <GameEngineOpenGL/ResourceManager.h>

Bullet::Bullet(glm::vec2 position, glm::vec2 direction,
	float velocity, int lifeTime)
{
	pos = position;
	dir = direction;
	speed = velocity;
	lifeT = lifeTime;
}
Bullet::~Bullet() {/*empty */ }

void Bullet::draw(ge::SpriteBatch & spriteBatch)
{
	static auto texture = ge::ResourceManager::getTexture(
		"Textures/JimmyJump_pack/PNG/Bubble_Small.png");
	glm::vec4 uv(0.f, 0.f, 1.f, 1.f);
	auto color= ge::ColorRGBA8(255, 255, 255, 255);

	auto posAndSize = glm::vec4(pos.x, pos.y, 5.f, 5.f);

	// drawing sprites
	spriteBatch.draw(posAndSize, uv, texture.id, 0.f /* depth */, color);
}

bool Bullet::update()
{
	pos += dir * speed;

	if (0 == --lifeT) {
		return true;
	}
	return false;
}
