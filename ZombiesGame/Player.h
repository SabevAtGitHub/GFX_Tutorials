#pragma once
#include "Human.h"
#include <GameEngineOpenGL\InputManager.h>
#include <GameEngineOpenGL\Camera2D.h>
#include "Bullet.h"

class Gun;

/// <summary>
/// Human controled by the player
/// </summary>
class Player :
	public Human
{
public:
	Player(glm::vec2 position, glm::vec2 direction,
		float initialSpeed, int numLives,
		ge::InputManager* inputManager, 
		ge::Camera2D* camera2D,
		std::vector<Bullet>* playerBullets);
	~Player();

	void addGun(Gun* gun);
	
	void update(const std::vector<std::string>& lvlData,
				std::vector<Human*>& humans,
				std::vector<Zombie*>& zombies,
				float deltaTime) override;
private:

	ge::InputManager* inputMngr_;
	std::vector<Gun*> guns_;
	ge::Camera2D* camera_;
	std::vector<Bullet>* bullets_;

	int currGunIdx_;
};

