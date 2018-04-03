#pragma once
#include "GLTexture.h"
#include <glm\glm.hpp>

namespace ge
{
	class TileSheet
	{
	public:
		void init(const GLTexture& texture, glm::ivec2 tileDims);
		glm::vec4 getUVs(int idx);

	public:
		ge::GLTexture texture;
		glm::ivec2 dims;
	};


	void TileSheet::init(const ge::GLTexture & texture, glm::ivec2 tileDims)
	{
		this->texture = texture;
		this->dims = tileDims;
	}

	glm::vec4 TileSheet::getUVs(int idx)
	{
		int xTile = idx % dims.x;
		int yTile = idx / dims.x;

		glm::vec4 uv;
		uv.x = xTile / (float)dims.x;
		uv.y = yTile / (float)dims.y;
		uv.z = 1.0f / (float)dims.x;
		uv.w = 1.0f / (float)dims.y;
		return uv;
	}

}