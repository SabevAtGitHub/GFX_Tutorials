#version 130
// The vertex shader operates on each vertex

// input data from the VBO. Each vertex is 2 floats
in vec2 vertexPos;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPos;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 P;

void main ()
{
	//Set the x,y position on the screen
	gl_Position.xy = (P * vec4(vertexPos, 0.0 , 1.0)).xy;
	gl_Position.z = 0.0; // z is for 3D
	
	// Indicates that the coordinates are normalized
	gl_Position.w = 1.0;
	
	fragmentPos = vertexPos;
	
	fragmentColor = vertexColor;
	
	fragmentUV = vertexUV;
}