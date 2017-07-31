#version 130
// The fragment shader operates on each pixel in a given polygon

//in vec2 fragPosition;
in vec4 fragColor;
in vec2 fragUV;


// 3 component float vector that gets outputed to the screen
// for each pixel.
out vec4 color;

uniform sampler2D pngTexture;

void main ()
{ 
	// rgb color from the sampler
	vec4 textureColor = texture(pngTexture, fragUV);
	
	color = fragColor * textureColor; 
}