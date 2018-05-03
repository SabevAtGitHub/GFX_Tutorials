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
	int getWidth() const { return m_levelData[0].size(); }
	int getHeight() const { return m_levelData.size(); }
	int getNumHumans() const { return m_numHumans; }

	glm::vec2 getPlayerStartPos() const { return m_playerStartPos; }
	const std::vector<glm::vec2>& getZombiesStartPos() const { return m_zombiesStartPos; }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
private:

	std::vector<std::string> m_levelData;
	ge::SpriteBatch m_spriteBatch;

	glm::vec2 m_playerStartPos;
	std::vector<glm::vec2> m_zombiesStartPos;
	int m_numHumans;
};

