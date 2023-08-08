#version 330 core

in vec2 vTex;

out vec4 fColor;

uniform sampler2D screenTex;

void main(){	

	// Out

	float depthValue = texture(screenTex, vTex).r;
	fColor = vec4(vec3(depthValue), 1.0);
	
}