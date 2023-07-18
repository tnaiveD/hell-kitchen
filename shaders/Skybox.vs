#version 420 core

out vec3 vPos;

layout(location = 0) in vec3 inPos;
layout(std140, binding = 0) uniform Matrices
{
	mat4 bProjection;
	mat4 bView;
};

void main(){
	vPos = inPos;
	vec4 pos = bProjection * mat4(mat3(bView)) * vec4(inPos, 1.0);
	gl_Position = pos.xyww;
}