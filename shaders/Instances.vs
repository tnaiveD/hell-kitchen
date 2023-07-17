#version 420 core

out vec3 vPos;
out vec3 vNormals;
out vec2 vTex;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormals;
layout(location = 2) in vec2 inTex;
layout(location = 3) in mat4 inModel;

layout(std140, binding = 0) uniform Matrices
{
	mat4 bProjection;
	mat4 bView;
};

void main()
{
	gl_Position = bProjection * bView * inModel * vec4(inPos, 1.0);

	vPos = vec3(inModel * vec4(inPos, 1.0));
	vNormals = mat3(inverse(transpose(inModel))) * inNormals;
	vTex = inTex;
}