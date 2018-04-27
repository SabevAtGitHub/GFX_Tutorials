#version 130
// The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPos;
in vec4 fragmentColor;
in vec2 fragmentUV;

// 4 component float vector that gets outputed to the screen
// for each pixel and the alpha.
out vec4 color;

//uniform float time; // uniform means constant
uniform sampler2D mySampler; // variable for the texture

void main ()
{ 
	vec4 textureColor = texture(mySampler, fragmentUV);
	
	// rgb color from the sampler	
	//color = textureColor * fragmentColor;
	 
	// making crazy colors
	
	color = fragmentColor * textureColor; /* vec4(fragmentColor.r) * (cos(fragmentPos.x * 3 + time) + 1.0) * 0.5,
			fragmentColor.g * (cos(fragmentPos.y * 8 + time) + 1.0) * 0.5,
			fragmentColor.b * (cos(fragmentPos.x * 2 + time) + 1.0) * 0.5,
			fragmentColor.a) * textureColor; */

	// ******************			

	//fragmentColor + vec4(1.0 * (cos(time) + 1.0) * 0.5, 
	//							1.0 * (cos(time+ 2) + 1.0) * 0.5,
	//							1.0 * (sin(time + 1.0) + 1.0) * 0.5,
	//							0.0);
	// ******************
}