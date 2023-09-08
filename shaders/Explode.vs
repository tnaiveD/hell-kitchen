#version 420 core

out vec3 vPos;
out vec3 vNormals;
out vec2 vTex;

out VS_OUT 
{
	vec2 texCoord;

} vs_out;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormals;
layout(location = 2) in vec2 inTex;

layout(std140, binding = 0) uniform Matrices
{
	mat4 bProjection;
	mat4 bView;
};

uniform mat4 vuModel;

void main(){
	vPos = vec3(vuModel * vec4(inPos, 1.0));
	vNormals = mat3(transpose(inverse(vuModel))) * inNormals;
	vs_out.texCoord = inTex;
	gl_Position = bProjection * bView * vuModel * vec4(inPos, 1.0);
}