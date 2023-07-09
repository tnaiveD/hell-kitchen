#version 420 core

out vec2 vTex;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTex;

layout(std140, binding = 0) uniform Matrices
{
	mat4 bProjection;
	mat4 bView;
};

uniform mat4 vuModel;

void main(){

	vTex = inTex;
	gl_Position = bProjection * bView * vuModel * vec4(inPos, 1.0);

}