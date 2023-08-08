#version 420 core

layout(location = 0) in vec3 inPos;

uniform mat4 vuModel;
uniform mat4 vuLightSpace;

void main()
{
	gl_Position = vuLightSpace * vuModel * vec4(inPos, 1.0);
	
}