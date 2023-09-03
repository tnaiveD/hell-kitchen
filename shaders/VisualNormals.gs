#version 420 core

///////////////////////

layout(line_strip, max_vertices = 6) out;

///////////////////////

layout(triangles) in;
layout(std140, binding = 0) uniform Matrices
{
    mat4 bProjection;
    mat4 bView;
};


in VS_OUT
{
    vec3 normal;
} gs_in[];

const float c_magnitude = 0.1;

void generateNormalLine(int index)
{
    gl_Position = bProjection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = bProjection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * c_magnitude);
    EmitVertex();
    EndPrimitive();
}


void main()
{   
    generateNormalLine(0);
    generateNormalLine(1);
    generateNormalLine(2);
}