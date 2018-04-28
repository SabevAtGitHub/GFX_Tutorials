#version 130
// The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPos;
in vec4 fragmentColor;
in vec2 fragmentUV;

// 4 component float vector that gets outputed to the screen
// for each pixel and the alpha.
out vec4 color;

// uniform sampler2D mySampler; // variable for the texture

void main ()
{ 
	// vec4 textureColor = texture(mySampler, fragmentUV); // to create some texture
	
	float distance = length(fragmentUV);
	color = vec4(fragmentColor.rgb, fragmentColor.a * pow( 0.01 , distance - 0.01 ));
}