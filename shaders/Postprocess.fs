#version 330 core

in vec2 vTex;

out vec4 fColor;

uniform sampler2D screenTex;

void main(){	
	//negative
	//---------------------
	//fColor = vec4(vec3(1.0 - texture(screenTex, vTex)), 1.0);
	
	//greyscale
	//---------------------
	vec3 colorTex = vec3(texture(screenTex, vTex));

	float avg = (colorTex.r + colorTex.g + colorTex.b) / 3.0;
	//or realistic below
	//float avg = 0.2126 * colorTex.r + 0.7152 * colorTex.g + 0.0722 * colorTex.b;
    
	//kernel effects
	//---------------------
	//ivec2 texSize = textureSize(screenTex, 0);
	float offset = 1.0 / 1000.0;

	vec2 offsets[9] = vec2[](
		vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bootom-right
	);

	//sharpen
	//float kernel[9] = float[](
	//	0, -1, 0,
    //   -1,  5, -1,
    //    0, -1, 0
	//);

	//blur
	float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);

	//edge detection
	//float kernel[9] = float[](
	//	0.0, -1.0, 0.0,
	//	-1.0, 4.0, -1.0,
	//	0.0, -1.0, 0.0
	//);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTex, vTex.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];


	//out
	//---------------------
	fColor = vec4(colorTex * avg, 1.0);
	
}