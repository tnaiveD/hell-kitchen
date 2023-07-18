#version 330 core

in vec2 vTex;

out vec4 fColor;

uniform sampler2D screenTex;

void main(){	
	
	//out
	//---------------------
	fColor = texture(screenTex, vTex);
	
}