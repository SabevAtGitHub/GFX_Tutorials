#include "SpriteBatch.h"
#include <algorithm>

namespace ge {
	/// <summary>
	///  To be able to draw bathces of textures
	/// </summary>
	SpriteBatch::SpriteBatch() : vbo(0), vao(0) {}
	
	SpriteBatch::~SpriteBatch()  { }

	void SpriteBatch::init()
	{
		createVertexArray();
	}
	void SpriteBatch::begin(GlyphSortType sortBy /* GlyphSortType::TEXTURE */)
	{ /// to setup any state before rendering
		
		// how to sort the Glyphs
		sortType = sortBy;
		renderBatches.clear();
		glyphs.clear();
	}
	void SpriteBatch::end()
	{
		// resize and retarget
		glyphPtrs.resize(glyphs.size());
		for (size_t i = 0; i < glyphPtrs.size(); i++) {
			glyphPtrs[i] = &glyphs[i];
		}

		sortGlyphs();
		createRenderBatches();
	}
	void SpriteBatch::draw(const glm::vec4 & destRect, 
		const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color)
	{
		glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}
	void SpriteBatch::renderBatch()
	{
		// binding vertex array object
		glBindVertexArray(vao);

		// drawing each batch
		for (size_t i = 0; i < renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, renderBatches[i].offset, renderBatches[i].numVertices);
		}

		// unbinding the array objects
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		if (glyphs.empty()) { return; }

		// vertex array to be uploaded to vbo
		// alocating all the memory we need for all
		// vertices for the glyphs
		// this means it can be treated as an array
		// and this is a lot faster than push back new item every time
		std::vector <Vertex> vertices;
		vertices.resize(glyphs.size() * 6);

		int offset = 0;
		int cv = 0; // current vertex

		// storing the first batch
		renderBatches.emplace_back(offset, 6, glyphPtrs[0]->texture);
		offset += 6;

		vertices[cv++] = glyphPtrs[0]->topLeft;
		vertices[cv++] = glyphPtrs[0]->bottomLeft;
		vertices[cv++] = glyphPtrs[0]->bottomRight;
		vertices[cv++] = glyphPtrs[0]->bottomRight;
		vertices[cv++] = glyphPtrs[0]->topRight;
		vertices[cv++] = glyphPtrs[0]->topLeft;

		// setting the rest of the batches
		for (size_t cg = 1; cg < glyphPtrs.size(); cg++) {
			if (glyphPtrs[cg]->texture != glyphPtrs[cg-1]->texture) {
				renderBatches.emplace_back(offset, 6, glyphPtrs[cg]->texture);
			}
			else {
				renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = glyphPtrs[cg]->topLeft;
			vertices[cv++] = glyphPtrs[cg]->bottomLeft;
			vertices[cv++] = glyphPtrs[cg]->bottomRight;
			vertices[cv++] = glyphPtrs[cg]->bottomRight;
			vertices[cv++] = glyphPtrs[cg]->topRight;
			vertices[cv++] = glyphPtrs[cg]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, 
			vertices.size() * sizeof(Vertex), nullptr,  GL_DYNAMIC_DRAW);
		// uploading the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, 
			vertices.size() * sizeof(Vertex), vertices.data());
		// unbinding the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void SpriteBatch::createVertexArray()
	{
		if (0 == vao) {
			glGenVertexArrays(1, &vao);
		}
		glBindVertexArray(vao);

		if (0 == vbo) {
			glGenBuffers(1, &vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Telling OpenGL what kind of attributes we're sending
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Passing more info (for flexibility if we don't want to draw the entire array)
		// This is the position attribute pointer 
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void *)offsetof(Vertex, pos));

		// This is the color attribute pointer 
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,
			sizeof(Vertex), (void *)offsetof(Vertex, color));

		// this is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void *)offsetof(Vertex, uv));
		
		// unbinding and disableing vao and vbo
		glBindVertexArray(0);
	}
	void SpriteBatch::sortGlyphs()
	{
		switch (sortType) {
		case ge::GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(glyphPtrs.begin(), glyphPtrs.end(), 
				[] (Glyph* a, Glyph* b) {return a->depth < b->depth; });
			break;
		case ge::GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(glyphPtrs.begin(), glyphPtrs.end(), 
				[] (Glyph* a, Glyph* b) {return a->depth > b->depth; });
			break;
		case ge::GlyphSortType::TEXTURE:
			std::stable_sort(glyphPtrs.begin(), glyphPtrs.end(), 
				[] (Glyph* a, Glyph* b) {return a->texture < b->texture; });
			break;
		}
		
	}

} //namespace ge
