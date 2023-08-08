#version 420 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTex;

layout(std140, binding = 0) uniform Matrices
{
	mat4 bProjection;
	mat4 bView;
};

uniform mat4 vuModel;
uniform mat4 vuLightSpace;

out VS_OUT 
{

	vec3 pos;
	vec3 normal;
	vec2 tex;
	vec4 lightSpacePos;

} out vs_out;

void main()
{
	vs_out.pos = vec3(vuModel * vec4(inPos, 1.0));
	vs_out.normal = transpose(inverse(mat3(vuModel))) * inNormal;
	vs_out.tex = inTex;
	vs_out.lightSpacePos = vuLightSpace * vec4(vs_out.pos, 1.0);

	gl_Position = bProjection * bView * vec4(vs_out.pos, 1.0);
}