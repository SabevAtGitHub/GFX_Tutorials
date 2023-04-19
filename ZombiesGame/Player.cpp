#include "Player.h"
#include "Gun.h"
#include <GameEngineOpenGL/ResourceManager.h>
#include <SDL/SDL.h>


Player::Player(glm::vec2 position, glm::vec2 direction,
				float initialSpeed, int numLives, 
				ge::InputManager* inputManager,
				ge::Camera2D* camera2D,
				std::vector<Bullet> *playerBullets)
{
	m_pos = position;
	m_dir = direction;
	m_speed = initialSpeed;
	m_inputManager = inputManager;
	m_camera2D = camera2D;
	m_bullets = playerBullets;
	m_currGunIndex = -1;

	// hardcoded color blue
	m_color.setColor(255, 255, 255, 255);
	m_textureId = ge::ResourceManager::getTexture("Textures/player.png").id;
}


Player::~Player()
{
	for (size_t i = 0; i < m_guns.size(); i++) {
		delete m_guns[i];
	}
}

void Player::addGun(Gun * gun)
{
	// adding gun to player's inventory
	m_guns.push_back(gun);

	// if no gun assigned as current, assign this gun
	if (-1 == m_currGunIndex) {
		m_currGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& lvlData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	// processing input - todo - move it from here someday
	if (m_inputManager->isKeyDown(SDLK_w)) {
		m_pos.y += m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_s)) {
		m_pos.y -= m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_d)) {
		m_pos.x += m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_a)) {
		m_pos.x -= m_speed * deltaTime;
	}

	// sycle trough guns
	if (m_inputManager->isKeyDown(SDLK_1) && m_guns.size() >= 0) {
		m_currGunIndex = 0;
	}
	if (m_inputManager->isKeyDown(SDLK_2) && m_guns.size() >= 1) {
		m_currGunIndex = 1;
	}
	if (m_inputManager->isKeyDown(SDLK_3) && m_guns.size() >= 2) {
		m_currGunIndex = 2;
	}

	auto mouseCoords = m_inputManager->getMouseCoords();
	mouseCoords = m_camera2D->covertScreenToWorld(mouseCoords);

	auto playerCenterPos = this->m_pos;// +glm::vec2(AGENT_RADIUS);

	m_dir = glm::normalize(mouseCoords - playerCenterPos);

	if (-1 != m_currGunIndex) {


		m_guns[m_currGunIndex]->fire(m_inputManager->isKeyDown(SDL_BUTTON_LEFT),
			playerCenterPos, m_dir, *m_bullets, deltaTime);
	}

	this->collideWithLevel(lvlData);
}

