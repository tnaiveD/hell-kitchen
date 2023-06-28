#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

out vec3 vPos;
out vec3 vNormal;

uniform mat4 vuModel;
uniform mat4 vuView;
uniform mat4 vuProjection;

void main(){
	vPos = vec3(vuModel * vec4(inPos, 1.0));
	vNormal = mat3(transpose(inverse(vuModel))) * inNormal;
	gl_Position = vuProjection * vuView * vec4(vPos, 1.0);
}