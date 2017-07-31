#pragma once
#include <string>
#include <vector>
#include <GameEngineOpenGL\SpriteBatch.h>

const float TILE_WITH = 64.f;
const float TILE_RADIUS = TILE_WITH / 2.f;

class Level
{
public:
	Level(const std::string& filePath);
	~Level();

	void draw();

	// getters
	int getWidth() const { return levelData_[0].size(); }
	int getHeight() const { return levelData_.size(); }
	int getNumHumans() const { return numHumans_; }

	glm::vec2 getPlayerStartPos() const { return playerStartPos_; }
	const std::vector<glm::vec2>& getZombiesStartPos() const { return zombiesStartPos_; }
	const std::vector<std::string>& getLevelData() const { return levelData_; }
private:

	std::vector<std::string> levelData_;
	ge::SpriteBatch spriteBatch_;

	glm::vec2 playerStartPos_;
	std::vector<glm::vec2> zombiesStartPos_;
	int numHumans_;
};

