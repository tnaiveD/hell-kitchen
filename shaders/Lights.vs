#version 420 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTex;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTex;

layout(std140, binding = 0) uniform Matrices
{
	mat4 bProjection;
	mat4 bView;
};

uniform mat4 vuModel;

void main(){
	vPos = vec3(vuModel * vec4(inPos, 1.0));
	vNormal = mat3(transpose(inverse(vuModel))) * inNormal;
	vTex = inTex;
	gl_Position = bProjection * bView * vuModel * vec4(inPos, 1.0);
}