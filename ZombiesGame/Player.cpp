#include "Player.h"
#include "Gun.h"
#include <GameEngineOpenGL\ResourceManager.h>
#include <SDL\SDL.h>


Player::Player(glm::vec2 position, glm::vec2 direction,
				float initialSpeed, int numLives, 
				ge::InputManager* inputManager,
				ge::Camera2D* camera2D,
				std::vector<Bullet> *playerBullets)
{
	pos_ = position;
	dir_ = direction;
	speed_ = initialSpeed;
	inputMngr_ = inputManager;
	camera_ = camera2D;
	bullets_ = playerBullets;
	currGunIdx_ = -1;

	// hardcoded color blue
	color_.setColor(255, 255, 255, 255);
	textureId_ = ge::ResourceManager::getTexture("Textures/player.png").id;
}


Player::~Player()
{
	for (size_t i = 0; i < guns_.size(); i++) {
		delete guns_[i];
	}
}

void Player::addGun(Gun * gun)
{
	// adding gun to player's inventory
	guns_.push_back(gun);

	// if no gun assigned as current, assign this gun
	if (-1 == currGunIdx_) {
		currGunIdx_ = 0;
	}
}

void Player::update(const std::vector<std::string>& lvlData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	// processing input - todo - move it from here someday
	if (inputMngr_->isKeyDown(SDLK_w)) {
		pos_.y += speed_ * deltaTime;
	}
	if (inputMngr_->isKeyDown(SDLK_s)) {
		pos_.y -= speed_ * deltaTime;
	}
	if (inputMngr_->isKeyDown(SDLK_d)) {
		pos_.x += speed_ * deltaTime;
	}
	if (inputMngr_->isKeyDown(SDLK_a)) {
		pos_.x -= speed_ * deltaTime;
	}

	// sycle trough guns
	if (inputMngr_->isKeyDown(SDLK_1) && guns_.size() >= 0) {
		currGunIdx_ = 0;
	}
	if (inputMngr_->isKeyDown(SDLK_2) && guns_.size() >= 1) {
		currGunIdx_ = 1;
	}
	if (inputMngr_->isKeyDown(SDLK_3) && guns_.size() >= 2) {
		currGunIdx_ = 2;
	}

	if (-1 != currGunIdx_) {
		auto playerCenterPos = this->pos_;// +glm::vec2(AGENT_RADIUS);
		auto mouseCoords = camera_->covertScreenToWorld(inputMngr_->getMouseCoords());
		auto direction = glm::normalize(mouseCoords - playerCenterPos);

		guns_[currGunIdx_]->fire(inputMngr_->isKeyDown(SDL_BUTTON_LEFT),
			playerCenterPos, direction, *bullets_, deltaTime);
	}

	this->collideWithLevel(lvlData);
}

