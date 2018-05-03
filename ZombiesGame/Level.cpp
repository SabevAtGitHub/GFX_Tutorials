#include "Level.h"
#include <fstream>
#include <iostream>
#include <GameEngineOpenGL\ErrManager.h>
#include <GameEngineOpenGL\ResourceManager.h>


Level::Level(const std::string & filePath)
{
	// opening the level file
	std::ifstream file;
	file.open(filePath);

	if (file.fail()) {
		ge::fatalError("Failed to open file " +
			filePath + "!\n");
	}

	// reading the first line,
	// trowing the text and reading the number of humans
	std::string tmp;
	file >> tmp >> m_numHumans;

	// trow away the first line
	std::getline(file, tmp); 

	// reading level data
	while (std::getline(file, tmp)) {
		m_levelData.push_back(tmp);
	}

	m_spriteBatch.init();
	m_spriteBatch.begin();
	
	const auto uvRect = glm::vec4(0.f, 0.f, 1.f, 1.f);
	ge::ColorRGBA8 color;
	color.setColor(255, 255, 255, 255);

	using rm = ge::ResourceManager;

	// reading level file in to array of arrays
	for (size_t y = 0; y < m_levelData.size(); y++) {
		for (size_t x = 0; x < m_levelData[y].size(); x++) {

			char tile = m_levelData[y][x];

			glm::vec4 destRect(x * (int)TILE_WITH, y * (int)TILE_WITH, (int)TILE_WITH, (int)TILE_WITH);

			// drawing the tiles
			switch (tile) {
			case '@':
				m_playerStartPos.x = x * TILE_WITH;
				m_playerStartPos.y = y * TILE_WITH;
				m_levelData[y][x] = '.'; // changing back to empty space
				break;
			case 'Z':
				m_zombiesStartPos.emplace_back(x * TILE_WITH, y * TILE_WITH);
				m_levelData[y][x] = '.'; // changing back to empty space
				break;
			case 'L':
				m_spriteBatch.draw(destRect, uvRect,
					rm::getTexture("Textures/L_bricks.png").id,
					0.f, color);
				break;
			case 'B':
				m_spriteBatch.draw(destRect, uvRect,
					rm::getTexture("Textures/B_bricks.png").id,
					0.f, color);
				break;
			case 'R':
				m_spriteBatch.draw(destRect, uvRect,
					rm::getTexture("Textures/R_bricks.png").id,
					0.f, color);
				break;
			case 'G':
				m_spriteBatch.draw(destRect, uvRect,
					rm::getTexture("Textures/G_bricks.png").id,
					0.f, color);
				break;
			case '.':
				break;
			default:
				std::printf("Unnown symbol %c at (%d, %d!", tile, x, y);
				break;
			}
		}
	}

	m_spriteBatch.end();
}

Level::~Level() { /* empty */ }

void Level::draw()
{
	m_spriteBatch.renderBatch();
}
