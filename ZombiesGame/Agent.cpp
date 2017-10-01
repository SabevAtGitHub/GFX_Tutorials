#include "Agent.h"
#include <GameEngineOpenGL\ResourceManager.h>
#include <algorithm> 
#include <iostream>

Agent::Agent() :
	pos_(0.f, 0.f),
	dir_(1.f, 0.f),
	speed_(0.f),
	lives_(1)
{ /* empty */ }

Agent::Agent(glm::vec2 position, glm::vec2 direction, 
	float speed, int numLives) :
	pos_(position),
	dir_(direction),
	speed_(speed)
{ /*empty*/ }

Agent::~Agent()
{ /*empty*/}

bool Agent::collideWithLevel(const std::vector<std::string>& lvlData)
{
	// Check the four corners of the tiles near the agent
	// get the center positions of each collidable tile
	// add add it to the the vector collisitonTilePos
	std::vector<glm::vec2> collideTileCenters;
	getCollisionTileCenter(this->pos_.x, this->pos_.y, collideTileCenters, lvlData);	// top left
	getCollisionTileCenter(this->pos_.x, this->pos_.y + AGENT_WIDTH, 
						   collideTileCenters, lvlData);							// bottom left
	getCollisionTileCenter(this->pos_.x + AGENT_WIDTH, 
						   this->pos_.y + AGENT_WIDTH, collideTileCenters, lvlData); // bottom right
	getCollisionTileCenter(this->pos_.x + AGENT_WIDTH, this->pos_.y, 
						   collideTileCenters, lvlData);							// top right

	if (0 == collideTileCenters.size()) {
		return false;
	}

	// sorting collistion tiles by distance	
	auto agentCenterPos = this->pos_ + glm::vec2(AGENT_RADIUS);
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
	auto thisCenterPos  = this->pos_ + glm::vec2(AGENT_RADIUS);
	auto agentCenterPos = agent->getPos() + glm::vec2(AGENT_RADIUS);

	auto distVec = thisCenterPos - agentCenterPos;
	auto collisionDepth = MIN_DISTANCE - glm::length(distVec);

	// if collision occured
	if (0 < collisionDepth) {
		// normalizing distVec to get the direction
		auto pushVec = glm::normalize(distVec) * (collisionDepth / 2.f);
		// pushing each agent with half depth in the oposite direction
		this->pos_ += pushVec;
		agent->pos_ -= pushVec;
		return true;
	}
	else {
		return false;
	}
}

void Agent::draw(ge::SpriteBatch & spriteBatch)
{
	const auto uvRect = glm::vec4(0.f, 0.f, 1.f, 1.f);
	glm::vec4 destRect(pos_.x , pos_.y, AGENT_WIDTH, AGENT_WIDTH);

	// drawing
	spriteBatch.draw(destRect, uvRect, textureId_, 0.f, color_, dir_);
}

bool Agent::applyDamage(float damage)
{
	health_ -= damage;
	return health_ <= 0;
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
	auto agentCenterPos = this->pos_ + glm::vec2(AGENT_RADIUS);
	auto distVec = agentCenterPos - tileCenterPos;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	/* if collision occured */
	if (xDepth > 0 && yDepth > 0 ) {
		// checking in wich direction to move the agent
		if (std::fmax(xDepth, 0.f) < std::fmax(yDepth, 0.f)) {
			if (distVec.x < 0) {
				this->pos_.x -= xDepth;
			}
			else {
				this->pos_.x += xDepth;
			}
		}
		else {
			if (distVec.y < 0) {
				this->pos_.y -= yDepth;
			}
			else {
				this->pos_.y += yDepth;
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

