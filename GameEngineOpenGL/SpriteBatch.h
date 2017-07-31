#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include "Vertex.h"
#include <vector>
#include <memory>


namespace ge {

	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class RenderBatch {
	public:
		RenderBatch::RenderBatch(GLuint offsetVal, GLuint verticesCount, GLuint textureId) : 
			offset(offsetVal), numVertices(verticesCount), texture(textureId) {}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class Glyph {
	public:
		Glyph() {};
		Glyph(const glm::vec4 & destRect,
			const glm::vec4 & uvRect, GLuint Texture, float Depth, const ColorRGBA8 & color) :
		texture(Texture), depth(Depth)
		{
			this->topLeft.color = color;
			this->topLeft.setPos(destRect.x, destRect.y + destRect.w);
			this->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			this->bottomLeft.color = color;
			this->bottomLeft.setPos(destRect.x, destRect.y);
			this->bottomLeft.setUV(uvRect.x, uvRect.y);

			this->bottomRight.color = color;
			this->bottomRight.setPos(destRect.x + destRect.z, destRect.y);
			this->bottomRight.setUV(uvRect.x + uvRect.w, uvRect.y);

			this->topRight.color = color;
			this->topRight.setPos(destRect.x + destRect.z, destRect.y + destRect.w);
			this->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		}

		GLuint texture;
		float depth;

		Vertex topLeft, bottomLeft, topRight, bottomRight;
	};

	class SpriteBatch /// To be able to draw bathces of textures
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType sortBy = GlyphSortType::TEXTURE);
		void end();
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		GLuint vbo;
		GLuint vao;
		GlyphSortType sortType;

		std::vector <Glyph*> glyphPtrs;
		std::vector <Glyph> glyphs;
		std::vector <RenderBatch> renderBatches;

	};
} //namespace ge