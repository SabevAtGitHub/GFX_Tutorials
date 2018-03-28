#pragma once
#include "GLSLProgram.h"
#include "Vertex.h"
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

		void drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
		void drawCircle(const glm::vec4& center, const ColorRGBA8& color, float radius);

		void render();

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
		GLuint m_vbo, m_vao, m_ibo;
	};


}