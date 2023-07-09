#version 420 core

out vec4 fColor;

in vec2 vTex;

uniform sampler2D tex;


void main(){	

	fColor = texture(tex, vTex);
	
}