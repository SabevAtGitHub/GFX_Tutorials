#pragma once
#include <glm/glm.hpp>
#include <GameEngineOpenGL/SpriteBatch.h>
#include "Level.h"

const float AGENT_WIDTH = 60.f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.f;

class Human;
class Zombie;

class Agent
{
public:
	Agent();
	Agent(glm::vec2 position, glm::vec2 direction, 
		float speed, int numLives);
	virtual ~Agent();

	/// <summary>
	/// purely virtual function, every derived class
	/// needs to implement it
	/// </summary>
	virtual void update(
		const std::vector<std::string>& lvlData, 
		std::vector<Human*>& humans, 
		std::vector<Zombie*>& zombies,
		float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& lvlData);

	bool collideWithAgent(Agent* agent);

	void draw(ge::SpriteBatch& spriteBatch);

	/// <summary>
	/// /returning true if agent died
	/// </summary>
	bool applyDamage(float damage);

	// setters
	void setPos(glm::vec2 newPos) { m_pos = newPos; }
	void setDir(glm::vec2 newDir) { m_dir = newDir; }
	void setSpeed(float speed) { m_speed = speed; }

	// getters
	glm::vec2 getPos() const { return m_pos; }
	const float getRadius() const { return m_radius; }
protected:

	void collideWidthTile(const glm::vec2& tilePos);
	void getCollisionTileCenter(float cornerX, float cornerY,
		std::vector<glm::vec2>& tilesPos, 
		const std::vector<std::string>& lvlData);

	glm::vec2 m_pos;
	glm::vec2 m_dir;
	float m_speed;
	float m_health;
	int m_numLives;
	const float m_radius = 20.f;
	ge::ColorRGBA8 m_color; 
	GLuint m_textureId;
};

