#include "Zombie.h"
#include "Human.h"
#include <GameEngineOpenGL\ResourceManager.h>

Zombie::Zombie()
{ /*empty*/ }


Zombie::~Zombie()
{ /*empty*/ }

void Zombie::init(glm::vec2 initialPos, float initialSpeed)
{
	this->m_pos = initialPos;
	this->m_speed = initialSpeed;
	this->m_color.setColor(255, 255, 255, 255);
	this->m_health = 20.f;
	m_textureId = ge::ResourceManager::getTexture("Textures/zombie.png").id;
}

void Zombie::update(const std::vector<std::string>& lvlData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	// chase humans
	auto closestHuman = getNearestHuman(humans);
	if (nullptr != closestHuman) {
		m_dir = glm::normalize(closestHuman->getPos() - this->m_pos);
		this->m_pos += m_dir * this->m_speed * deltaTime;
	}

	// collide with level
	collideWithLevel(lvlData);
}

Human * Zombie::getNearestHuman(std::vector<Human*>& humans)
{
	Human* closestHuman = nullptr;
	float smallestDist = 9999999.f;

	for (size_t i = 0; i < humans.size(); i++) {		
		float dist = glm::length(humans[i]->getPos() - this->m_pos);
		
		if (dist < smallestDist) {
			smallestDist = dist;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}

