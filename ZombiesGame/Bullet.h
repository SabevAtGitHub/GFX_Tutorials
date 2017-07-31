#pragma once
#include <glm\glm.hpp>
#include <GameEngineOpenGL\SpriteBatch.h>
#include <vector>

class Agent;
class Human;
class Zombie;

const float BULLET_WIDTH = 10.f;
const float BULLET_RADIUS = BULLET_WIDTH / 2.f;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float bulletDamage, float bulletSpeed);
	~Bullet();

	bool update(const std::vector<std::string>& lvlData, float deltaTime);
	
	void draw(ge::SpriteBatch& spriteBatch);

	bool collideWithAgent(Agent* agent);

	// getters
	float getDamage() const { return damage_; }

	glm::vec2 getPos() const { return pos_; }
private:
	bool collideWithLevel(const std::vector<std::string>& lvlData);
	
	glm::vec2 pos_;
	glm::vec2 dir_;
	
	float damage_;
	float speed_;
};

