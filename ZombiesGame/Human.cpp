#include "Human.h"
#include <GameEngineOpenGL\ResourceManager.h>
#include <random>
#include <time.h>
#include <glm\gtx\rotate_vector.hpp>


Human::Human() : 
	frames_(0)
{
}

Human::~Human()
{
}

void Human::init(glm::vec2 initialPos, float initialSpeed)
{ 
	static std::mt19937 randEngine(static_cast<unsigned int>(time(nullptr)));
	static std::uniform_real_distribution<float> randDir(-1.f, 1.f);

	this->m_pos = initialPos;
	this->m_speed = initialSpeed;
	this->color_.setColor(255, 255, 255, 255);
	this->health_ =15.f;

	// getting random direction
	this->m_dir = glm::vec2(randDir(randEngine), randDir(randEngine));

	// just in case both randDir endup == 0
	if (0 == this->m_dir.length()) {
		this->m_dir = glm::vec2(1.f, 0.f);
	}

	this->m_dir = glm::normalize(this->m_dir);
	textureId_ = ge::ResourceManager::getTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& lvlData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	static std::mt19937 randEngine(static_cast<unsigned int>(time(nullptr)));
	static std::uniform_real_distribution<float> randRotate(-20.1f, 20.1f);

	this->m_pos += this->m_dir * this->m_speed * deltaTime;

	if (200 == frames_) {
		this->m_dir = glm::rotate(this->m_dir, randRotate(randEngine));
		frames_ = 0;
	}
	else {
		frames_++;
	}

	if (collideWithLevel(lvlData)) {
		this->m_dir = glm::rotate(this->m_dir, randRotate(randEngine));
		frames_ = 0;
	}
}
