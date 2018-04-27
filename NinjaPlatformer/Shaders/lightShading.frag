#version 130
// The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPos;
in vec4 fragmentColor;
in vec2 fragmentUV;

// 4 component float vector that gets outputed to the screen
// for each pixel and the alpha.
out vec4 color;


void main ()
{ 
	float distance = lenght(fragmentUV);
 
	color =vec4(fragmentColor.rgb, fragmentColor.a * (1.0 - distance);
}