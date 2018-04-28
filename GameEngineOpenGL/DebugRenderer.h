#pragma once
#include "GLSLProgram.h"
#include "Vertex.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>


namespace ge
{

	class DebugRenderer
	{
	public:
		DebugRenderer();
		~DebugRenderer();

		void init();

		void end();

		void render(const glm::mat4& projectionMatrix, float lineWidth);

		void drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
		void drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);

		void dispose();

	public:
		struct DebugVertex {
			glm::vec2 pos;
			ge::ColorRGBA8 color;
		};
	private:
		ge::GLSLProgram m_program;
		std::vector<DebugVertex> m_verts;
		std::vector<GLuint> m_indices;
		GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
		int m_numElements = 0;
	};


}