#pragma once
#include <GL\glew.h>

namespace ge {
	/// <summary>
	/// Located in Vertex.h
	/// </summary>
	struct Position
	{
		/// <summary>
		/// Located in Vertex.h, Defaults to (0, 0)
		/// </summary>
		Position() { setPosition(0.f, 0.f); }
		Position(float xPos, float yPos) { x = xPos; y = yPos; }
		float x, y;
		void setPosition(float xPos, float yPos) { x = xPos; y = yPos; }
	};

	/// <summary>
	/// Located in Vertex.h
	/// </summary>
	struct ColorRGBA8
	{
		/// <summary>
		/// Defaults to black color
		/// </summary>
		ColorRGBA8() { setColor(0, 0, 0, 255); } // black
		/// <summary>
		/// 4 GLubyte values - red, green, blue, alpha
		/// </summary>
		ColorRGBA8(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) 
			{ r = red; g = green; b = blue; a = alpha; }
		
		void setColor(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
			{ r = red; g = green; b = blue; a = alpha; }
		GLubyte r, g, b, a;
	};

	/// <summary>
	/// Located in Vertex.h
	/// </summary>
	struct UV
	{
		/// <summary>
		/// Located in Vertex.h, Defaults to (0, 0)
		/// </summary>
		UV() { u = 0.f, v = 0.f; }
		UV(float uCoord, float vCoord) { u = uCoord, v = vCoord; }
		float u, v;
	};

	struct Vertex
	{
		Position pos;
		ColorRGBA8 color;
		UV uv;

		void setPos(float xPos, float yPos) { pos = Position(xPos, yPos); }
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte alpha) { color = ColorRGBA8(r,  g,  b,  alpha); }
		void setUV(float u, float v) { uv = UV(u, v); }
	};
}