#include "Gun.h"
#include <random>
#include <time.h>
#include <glm\gtx\rotate_vector.hpp>

Gun::Gun(std::string gunName, int fireRate, int bulletsPerShot,
	float spread, float bulletsSpeed, float bulletsDamage,
	ge::SoundEffect soundEffect) :
	/*initializer list*/
	m_soundEffect(soundEffect),
	m_gunName(gunName),
	m_fireRate(fireRate),
	m_bulletsPerShot(bulletsPerShot),
	m_spread(spread),
	m_bulletsSpeed(bulletsSpeed),
	m_bulletsDamage(bulletsDamage),
	m_frameCounter(0)
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
	m_frameCounter += 1 * detaTime;
	// randomize shooting direction based on the spread
	std::mt19937 randEngine(static_cast<unsigned int>(time(nullptr)));
	std::uniform_real_distribution<float> randRotate(-m_spread, m_spread);

	if (m_frameCounter >= m_fireRate  && isMouseDown) {
		m_soundEffect.play();
		
		for (int i = 0; i < m_bulletsPerShot; i++) {
			bullets.emplace_back(position, 
								glm::rotate(direction, randRotate(randEngine)),
								m_bulletsDamage, m_bulletsSpeed);
		}
		m_frameCounter = 0.f;
	}
}
