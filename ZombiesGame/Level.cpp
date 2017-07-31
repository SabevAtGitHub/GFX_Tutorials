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
	file >> tmp >> numHumans_;

	// trow away the first line
	std::getline(file, tmp); 

	// reading level data
	while (std::getline(file, tmp)) {
		levelData_.push_back(tmp);
	}

	spriteBatch_.init();
	spriteBatch_.begin();
	
	const auto uvRect = glm::vec4(0.f, 0.f, 1.f, 1.f);
	ge::ColorRGBA8 color;
	color.setColor(255, 255, 255, 255);

	using rm = ge::ResourceManager;

	// reading level file in to array of arrays
	for (size_t y = 0; y < levelData_.size(); y++) {
		for (size_t x = 0; x < levelData_[y].size(); x++) {

			char tile = levelData_[y][x];

			glm::vec4 destRect(x * (int)TILE_WITH, y * (int)TILE_WITH, (int)TILE_WITH, (int)TILE_WITH);

			// drawing the tiles
			switch (tile) {
			case '@':
				playerStartPos_.x = x * TILE_WITH;
				playerStartPos_.y = y * TILE_WITH;
				levelData_[y][x] = '.'; // changing back to empty space
				break;
			case 'Z':
				zombiesStartPos_.emplace_back(x * TILE_WITH, y * TILE_WITH);
				levelData_[y][x] = '.'; // changing back to empty space
				break;
			case 'L':
				spriteBatch_.draw(destRect, uvRect,
					rm::getTexture("Textures/L_bricks.png").id,
					0.f, color);
				break;
			case 'B':
				spriteBatch_.draw(destRect, uvRect,
					rm::getTexture("Textures/B_bricks.png").id,
					0.f, color);
				break;
			case 'R':
				spriteBatch_.draw(destRect, uvRect,
					rm::getTexture("Textures/R_bricks.png").id,
					0.f, color);
				break;
			case 'G':
				spriteBatch_.draw(destRect, uvRect,
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

	spriteBatch_.end();
}

Level::~Level() { /* empty */ }

void Level::draw()
{
	spriteBatch_.renderBatch();
}
