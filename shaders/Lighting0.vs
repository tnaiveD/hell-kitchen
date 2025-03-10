#version 420 core

///////////////////////////////////////////
// Out
///////////////////

out VS_OUT
{
	vec3 pos;
	vec3 normal;
	vec2 tex;
	vec4 lightSpacePos;

} vs_out;

////////////////////////////////////////////
// In
///////////////////

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

////////////////////////////////////////////
// Stuff
///////////////////

void main()
{
	
	vs_out.pos = vec3(vuModel * vec4(inPos, 1.0));
	vs_out.normal = mat3(transpose(inverse(vuModel))) * inNormal;
	vs_out.tex = inTex;
	vs_out.lightSpacePos = vuLightSpace * vec4(vs_out.pos, 1.0);	

	gl_Position = bProjection * bView * vec4(vs_out.pos, 1.0);
}

