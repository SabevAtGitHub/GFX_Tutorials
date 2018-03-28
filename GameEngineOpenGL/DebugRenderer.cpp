#include "DebugRenderer.h"

namespace ge
{
	DebugRenderer::DebugRenderer() { /* empty */ }

	DebugRenderer::~DebugRenderer()
	{
		dispose();
	}

	void DebugRenderer::init()
	{
		// Shader initialization


		// Set up buffers
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		// TODO: Vertex attrib pointers


		glBindVertexArray(0);
	}

	void DebugRenderer::end()
	{
	}

	void DebugRenderer::drawBox(const glm::vec4 & destRect, const ColorRGBA8 & color, float angle)
	{
	}

	void DebugRenderer::drawCircle(const glm::vec4 & center, const ColorRGBA8 & color, float radius)
	{
	}

	void DebugRenderer::render()
	{
	}

	void DebugRenderer::dispose()
	{
	}

}