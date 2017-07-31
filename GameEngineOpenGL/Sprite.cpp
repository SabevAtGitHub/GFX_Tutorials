#include "Sprite.h"
#include "Vertex.h"
#include <cstddef>
#include "ResourceManager.h"

namespace ge {

	Sprite::Sprite() :
		vboId(0) {}

	Sprite::~Sprite()
	{
		if (0 != vboId) {
			// just in case, free the buffer
			glDeleteBuffers(1, &vboId);
		}
	}

	void Sprite::init(float xPos, float yPos, float width, float height,
		std::string texturePath)
	{
		x = xPos;
		y = yPos;
		w = width;
		h = height;

		texture = ResourceManager::getTexture(texturePath);

		// creating the vertex buffer
		// generates buffer and is changing
		// the vboId to the buffer id 
		if (0 == vboId) {			
			glGenBuffers(1, &vboId);
		}

		Vertex vertexData[12]; // 6 vertecies with 2 floats

		// first triangle
		vertexData[0].setPos(x + w, y + h); // top right
		vertexData[0].setUV(1.0f, 1.0f);
		vertexData[1].setPos(x, y + h);     // top left
		vertexData[1].setUV(0.0f, 1.0f);
		vertexData[2].setPos(x, y);         // bottom left
		vertexData[2].setUV(0.0f, 0.0f);

		// second triangle
		vertexData[3].setPos(x, y);         // bottom left
		vertexData[3].setUV(0.0f, 0.0f);
		vertexData[4].setPos(x + w, y);		// bottom right
		vertexData[4].setUV(1.0f, 0.0f);
		vertexData[5].setPos(x + w, y + h); // top right
		vertexData[5].setUV(1.0f, 1.0f);

		for (auto i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 0, 255, 255);
		}

		vertexData[1].setColor(0, 0, 255, 255);
		vertexData[4].setColor(0, 255, 0, 255);

		// to make activate this buffer
		glBindBuffer(GL_ARRAY_BUFFER, vboId);

		// passing the data for the buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData),
			vertexData, GL_STATIC_DRAW);

		// unbinding, not mandatory but safe
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/// <summary>
	/// Drawing a Sprite object
	/// </summary>
	void Sprite::draw()
	{
		// this specifies which texture were binding, since multiple
		// textures can be used at the same time in the shaders
		glBindTexture(GL_TEXTURE_2D, texture.id);

		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		   
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

		//Actual drawing
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// disableing the attributes
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
