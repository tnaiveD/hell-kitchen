#version 420 core

out VS_OUT
{
    vec3 normal;
} vs_out;

////////////////////////

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

////////////////////////

layout(std140, binding = 0) uniform Matrices
{
    mat4 bProjection;
    mat4 bView;
};

uniform mat4 vuModel;

void main()
{
    gl_Position = bView * vuModel * vec4(inPos, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(bView * vuModel)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * inNormal, 1.0)));
}


