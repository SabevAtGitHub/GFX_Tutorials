#include "DebugRenderer.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace ge
{
	namespace Shader
	{
#pragma region Vertex Shaders

		const char* VERT_SRC = R"(#version 130
				//The vertex shader operates on each vertex

				//input data from the VBO. Each vertex is 2 floats
				in vec2 vertexPosition;
				in vec4 vertexColor;

				out vec2 fragmentPosition;
				out vec4 fragmentColor;

				uniform mat4 P;

				void main() {
					//Set the x,y position on the screen
					gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
					//the z position is zero since we are in 2D
					gl_Position.z = 0.0;
    
					//Indicate that the coordinates are normalized
					gl_Position.w = 1.0;
    
					fragmentPosition = vertexPosition;
    
					fragmentColor = vertexColor;
				})";

#pragma endregion // Vertex Shaders

#pragma region Fragment Shaders

		const char* FRAG_SRC = R"(#version 130
				//The fragment shader operates on each pixel in a given polygon

				in vec2 fragmentPosition;
				in vec4 fragmentColor;

				//This is the 3 component float vector that gets outputted to the screen
				//for each pixel.
				out vec4 color;

				void main() {

					color = fragmentColor;
					})";

#pragma endregion // Fragment Shaders
	}


	DebugRenderer::DebugRenderer() { /* empty */ }

	DebugRenderer::~DebugRenderer()
	{
		dispose();
	}

	void DebugRenderer::init()
	{
		// Shader initialization
		m_program.compileShadersFromSource(Shader::VERT_SRC, Shader::FRAG_SRC);
		m_program.addAttribute("vertexPosition");
		m_program.addAttribute("vertexColor");
		m_program.linkShaders();

		// Set up buffers
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		// Vertex attrib pointers
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void *)offsetof(DebugVertex, pos));
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void *)offsetof(DebugVertex, color));

		glBindVertexArray(0);
	}
	
	void DebugRenderer::end()
	{
	// VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // bind
		
		// Orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);

		// Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_verts.size() * sizeof(DebugVertex), m_verts.data());
		
		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind

	// IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo); // bind

		// Orphan the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

		// Upload the data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind

	}

	glm::vec2 rotatePoint(const glm::vec2& pos, float angle) {
		glm::vec2 newVec;
		newVec.x = pos.x * cos(angle) - pos.y * sin(angle);
		newVec.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newVec;
	}

	void DebugRenderer::drawBox(const glm::vec4 & destRect, const ColorRGBA8 & color, float angle)
	{
		int i = m_verts.size();
		m_verts.resize(i + 4);

		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

		// Get points centered at origin
		glm::vec2 tl(-halfDims.x, halfDims.y);
		glm::vec2 bl(-halfDims.x, -halfDims.y);
		glm::vec2 br(halfDims.x, -halfDims.y);
		glm::vec2 tr(halfDims.x, halfDims.y);

		glm::vec2 offsetPos(destRect.x, destRect.y);

		// Rotate the points
		m_verts[i].pos = rotatePoint(tl, angle) + halfDims + offsetPos;
		m_verts[i + 1].pos = rotatePoint(bl, angle) + halfDims + offsetPos;
		m_verts[i + 2].pos = rotatePoint(br, angle) + halfDims + offsetPos;
		m_verts[i + 3].pos = rotatePoint(tr, angle) + halfDims + offsetPos;

		// adding the color
		for (int j = i; j < i + 4; j++) {
			m_verts[j].color = color;
		}

		// Setup indices for drawing a box
		m_indices.reserve(m_indices.size() + 8);

		m_indices.push_back(i);
		m_indices.push_back(i + 1);

		m_indices.push_back(i + 1);
		m_indices.push_back(i + 2);

		m_indices.push_back(i + 2);
		m_indices.push_back(i + 3);

		m_indices.push_back(i + 3);
		m_indices.push_back(i);
	}

	void DebugRenderer::drawCircle(const glm::vec4 & center, const ColorRGBA8 & color, float radius)
	{
		// setting up the vertices
		static const int NUM_VERTS = 100;
		int start = m_verts.size();

		for (int i = 0; i < NUM_VERTS; i++) {
			float angle = ((float)i / NUM_VERTS) * M_PI * 2.f;
			m_verts[start + i].pos.x + cos(angle) * radius;
			m_verts[start + i].pos.y + sin(angle) * radius;
			m_verts[start + i].color = color;
		}

		// settig up the indices
		m_indices.reserve(m_indices.size() + NUM_VERTS * 2);
		for (int i = 0; i < NUM_VERTS - 1; i++) {
			m_indices.push_back(start + i);
			m_indices.push_back(start + i + 1);
		}

		// last index wraps around to start
		m_indices.push_back(start + NUM_VERTS - 1);
		m_indices.push_back(start);
	}

	void DebugRenderer::render()
	{
	}

	void DebugRenderer::dispose()
	{
	}

}