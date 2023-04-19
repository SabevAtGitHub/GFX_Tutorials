#include "Agent.h"
#include <GameEngineOpenGL/ResourceManager.h>
#include <algorithm> 
#include <iostream>

Agent::Agent() :
	m_pos(0.f, 0.f),
	m_dir(1.f, 0.f),
	m_speed(0.f),
	m_numLives(1)
{ /* empty */ }

Agent::Agent(glm::vec2 position, glm::vec2 direction, 
	float speed, int numLives) :
	m_pos(position),
	m_dir(direction),
	m_speed(speed)
{ /*empty*/ }

Agent::~Agent()
{ /*empty*/}

bool Agent::collideWithLevel(const std::vector<std::string>& lvlData)
{
	// Check the four corners of the tiles near the agent
	// get the center positions of each collidable tile
	// add add it to the the vector collisitonTilePos
	std::vector<glm::vec2> collideTileCenters;
	getCollisionTileCenter(this->m_pos.x, this->m_pos.y, collideTileCenters, lvlData);	// top left
	getCollisionTileCenter(this->m_pos.x, this->m_pos.y + AGENT_WIDTH, 
						   collideTileCenters, lvlData);							// bottom left
	getCollisionTileCenter(this->m_pos.x + AGENT_WIDTH, 
						   this->m_pos.y + AGENT_WIDTH, collideTileCenters, lvlData); // bottom right
	getCollisionTileCenter(this->m_pos.x + AGENT_WIDTH, this->m_pos.y, 
						   collideTileCenters, lvlData);							// top right

	if (0 == collideTileCenters.size()) {
		return false;
	}

	// sorting collistion tiles by distance	
	auto agentCenterPos = this->m_pos + glm::vec2(AGENT_RADIUS);
	std::sort(collideTileCenters.begin(), collideTileCenters.end(),
		[&](glm::vec2& a, glm::vec2& b) 
			{return glm::length(agentCenterPos - a) < glm::length(agentCenterPos - b); });
	
	// collide with tile ( move agent accordingly)
	for (size_t i = 0; i < collideTileCenters.size(); i++) {
		collideWidthTile(collideTileCenters[i]);		
	}
	return true;
}

/// <summary>
/// circular collistion
/// </summary>
/// <param name="agent"></param>
/// <returns>If collision occured, so that zombies can convert humans</returns>
bool Agent::collideWithAgent(Agent * agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS * 2.f;
	auto thisCenterPos  = this->m_pos + glm::vec2(AGENT_RADIUS);
	auto agentCenterPos = agent->getPos() + glm::vec2(AGENT_RADIUS);

	auto distVec = thisCenterPos - agentCenterPos;
	auto collisionDepth = MIN_DISTANCE - glm::length(distVec);

	// if collision occured
	if (0 < collisionDepth) {
		// normalizing distVec to get the direction
		auto pushVec = glm::normalize(distVec) * (collisionDepth / 2.f);
		// pushing each agent with half depth in the oposite direction
		this->m_pos += pushVec;
		agent->m_pos -= pushVec;
		return true;
	}
	else {
		return false;
	}
}

void Agent::draw(ge::SpriteBatch & spriteBatch)
{
	const auto uvRect = glm::vec4(0.f, 0.f, 1.f, 1.f);
	glm::vec4 destRect(m_pos.x , m_pos.y, AGENT_WIDTH, AGENT_WIDTH);

	// drawing
	spriteBatch.draw(destRect, uvRect, m_textureId, 0.f, m_color, m_dir);
}

bool Agent::applyDamage(float damage)
{
	m_health -= damage;
	return m_health <= 0;
}

/// <summary>
/// AABB COLLISTION
/// calculates the distance (depth) between agents's center 
/// and tile's center and changes agent's positoin accordingly
/// </summary>
void Agent::collideWidthTile(const glm::vec2& tileCenterPos)
{
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	// finding the distance vector between tile center
	// and the agent center
	auto agentCenterPos = this->m_pos + glm::vec2(AGENT_RADIUS);
	auto distVec = agentCenterPos - tileCenterPos;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	/* if collision occured */
	if (xDepth > 0 && yDepth > 0 ) {
		// checking in wich direction to move the agent
		if (std::fmax(xDepth, 0.f) < std::fmax(yDepth, 0.f)) {
			if (distVec.x < 0) {
				this->m_pos.x -= xDepth;
			}
			else {
				this->m_pos.x += xDepth;
			}
		}
		else {
			if (distVec.y < 0) {
				this->m_pos.y -= yDepth;
			}
			else {
				this->m_pos.y += yDepth;
			}
		}
	}
}

/// <summary>
/// Filling tilesPos vector with the center positions of
/// all collidable tiles
/// </summary>
/// <param name="cornerX">Agent's corner X position</param>
/// <param name="cornerY">Agent's corner Y position</param>
/// <param name="collideTileCenters">Vector to hold all tiles positions, chanded by the function</param>
/// <param name="lvlData">To check if the tile is not empty</param>
void Agent::getCollisionTileCenter(float cornerX, float cornerY, 
				std::vector<glm::vec2>& collideTileCenters, 
				const std::vector<std::string>& lvlData)
{
	// getting tile's center
	cornerX = floor(cornerX / TILE_WITH);
	cornerY = floor(cornerY / TILE_WITH);

	// if we are outside of the world, just return
	if (cornerX < 0 || cornerX >= lvlData[0].size() ||
		cornerY < 0 || cornerY >= lvlData.size()) {
		return;
	}

	// if the tile is colidable, puch back the tile's center pos
	if ('.' != lvlData[(int)cornerY][(int)cornerX]) {
		collideTileCenters.push_back(glm::vec2(cornerX * TILE_WITH + TILE_WITH * 0.5f,
											   cornerY * TILE_WITH + TILE_WITH * 0.5f));
	}

}

