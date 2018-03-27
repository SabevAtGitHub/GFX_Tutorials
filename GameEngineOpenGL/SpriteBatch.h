#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL\glew.h>
#include "Vertex.h"

namespace ge {

	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph {
	public:
		Glyph() {};
		Glyph(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint m_texture, float depth, const ColorRGBA8 & color);
		
		Glyph(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint m_texture, float depth, const ColorRGBA8 & color, float angle);

		GLuint m_texture;
		float depth;

		Vertex topLeft, bottomLeft, topRight, bottomRight;
	private:
		// Rotates a point about (0,0) by angle
		glm::vec2 rotatePoint(const glm::vec2& pos, float angle);
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint offsetVal, GLuint verticesCount, GLuint textureId) : 
			offset(offsetVal), numVertices(verticesCount), m_texture(textureId) {}
		GLuint offset;
		GLuint numVertices;
		GLuint m_texture;
	};


	class SpriteBatch /// To be able to draw bathces of textures
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType sortBy = GlyphSortType::TEXTURE);
		void end();

		/// <summary>
		/// adds a Glyph to the sprite batch
		/// </summary>
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint m_texture, float depth, const ColorRGBA8& color);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint m_texture, float depth, const ColorRGBA8& color, float angle);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint m_texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);

		/// <summary>
		/// renders entire SpriteBatch
		/// </summary>
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