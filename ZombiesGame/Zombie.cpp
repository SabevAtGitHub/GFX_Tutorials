#include "Zombie.h"
#include "Human.h"
#include <GameEngineOpenGL\ResourceManager.h>

Zombie::Zombie()
{ /*empty*/ }


Zombie::~Zombie()
{ /*empty*/ }

void Zombie::init(glm::vec2 initialPos, float initialSpeed)
{
	this->pos_ = initialPos;
	this->speed_ = initialSpeed;
	this->color_.setColor(255, 255, 255, 255);
	this->health_ = 20.f;
	textureId_ = ge::ResourceManager::getTexture("Textures/zombie.png").id;
}

void Zombie::update(const std::vector<std::string>& lvlData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	// chase humans
	auto closestHuman = getNearestHuman(humans);
	if (nullptr != closestHuman) {
		dir_ = glm::normalize(closestHuman->getPos() - this->pos_);
		this->pos_ += dir_ * this->speed_ * deltaTime;
	}

	// collide with level
	collideWithLevel(lvlData);
}

Human * Zombie::getNearestHuman(std::vector<Human*>& humans)
{
	Human* closestHuman = nullptr;
	float smallestDist = 9999999.f;

	for (size_t i = 0; i < humans.size(); i++) {		
		float dist = glm::length(humans[i]->getPos() - this->pos_);
		
		if (dist < smallestDist) {
			smallestDist = dist;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}

