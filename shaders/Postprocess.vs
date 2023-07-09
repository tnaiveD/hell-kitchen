#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTex;

out vec2 vTex;

void main(){
	vTex = inTex;
	gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
}