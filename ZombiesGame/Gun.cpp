#include "Gun.h"
#include <random>
#include <time.h>
#include <glm\gtx\rotate_vector.hpp>

Gun::Gun(std::string gunName, int fireRate, int bulletsPerShot,
	float spread, float bulletsSpeed, float bulletsDamage,
	ge::SoundEffect soundEffect) :
	/*initializer list*/
	soundEffect_(soundEffect),
	gunName_(gunName),
	fireRate_(fireRate),
	bulletsPerShot_(bulletsPerShot),
	spread_(spread),
	bulletsSpeed_(bulletsSpeed),
	bulletsDamage_(bulletsDamage),
	frameCounter_(0)
{
	/*empty*/
}

Gun::~Gun()
{
	/*empty*/
}

void Gun::fire(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction,
	std::vector<Bullet>& bullets, float detaTime)
{
	frameCounter_ += 1 * detaTime;
	// randomize shooting direction based on the spread
	std::mt19937 randEngine(static_cast<unsigned int>(time(nullptr)));
	std::uniform_real_distribution<float> randRotate(-spread_, spread_);

	if (frameCounter_ >= fireRate_  && isMouseDown) {
		soundEffect_.play();
		
		for (int i = 0; i < bulletsPerShot_; i++) {
			bullets.emplace_back(position, 
								glm::rotate(direction, randRotate(randEngine)),
								bulletsDamage_, bulletsSpeed_);
		}
		frameCounter_ = 0.f;
	}
}
