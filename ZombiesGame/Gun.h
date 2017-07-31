#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <GameEngineOpenGL\AudioManager.h>

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
	ge::SoundEffect soundEffect_;

	std::string gunName_;

	int fireRate_; ///< Fire rate in terms of frames
	int bulletsPerShot_;
	float spread_;
	
	float bulletsSpeed_;
	float bulletsDamage_;
	float frameCounter_;
};

