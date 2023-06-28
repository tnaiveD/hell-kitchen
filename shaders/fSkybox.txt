#version 420 core

in vec3 vPos;

out vec4 fColor;

uniform samplerCube skyboxTex;

void main(){	
	fColor = texture(skyboxTex, vPos);
}