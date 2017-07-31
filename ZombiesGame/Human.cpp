#include "Human.h"
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
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.f, 1.f);

	this->pos_ = initialPos;
	this->speed_ = initialSpeed;
	this->color_.setColor(230, 30, 230, 255);
	this->health_ =15.f;

	// getting random direction
	this->dir_ = glm::vec2(randDir(randEngine), randDir(randEngine));

	// just in case both randDir endup == 0
	if (0 == this->dir_.length()) {
		this->dir_ = glm::vec2(1.f, 0.f);
	}

	this->dir_ = glm::normalize(this->dir_);
}

void Human::update(const std::vector<std::string>& lvlData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-20.1f, 20.1f);

	this->pos_ += this->dir_ * this->speed_ * deltaTime;

	if (200 == frames_) {
		this->dir_ = glm::rotate(this->dir_, randRotate(randEngine));
		frames_ = 0;
	}
	else {
		frames_++;
	}

	if (collideWithLevel(lvlData)) {
		this->dir_ = glm::rotate(this->dir_, randRotate(randEngine));
		frames_ = 0;
	}
}
