 #pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GameEngineOpenGL/AudioManager.h>

#include "Bullet.h"

class Gun
{
public:
	Gun(std::string gunName, int fireRate, int bulletsPerShot,
		float spread, float bulletSpeed, float bulletDamage,
		ge::SoundEffect soundEffect);
	~Gun();

	void fire(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, 
		std::vector<Bullet>& bullets, float detaTime);

private:
	ge::SoundEffect m_soundEffect;
	std::string m_gunName;

	int m_fireRate; ///< Fire rate in terms of frames
	int m_bulletsPerShot;
	float m_spread;
	float m_bulletsSpeed;
	float m_bulletsDamage;
	float m_frameCounter;
};

