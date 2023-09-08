#version 330 core

in vec2 vTex;

out vec4 fColor;

uniform sampler2D screenTex;
uniform float nearPlane;
uniform float farPlane;

float linerizeDepth(float depthValue)
{
	float z = depthValue * 2.0 - 1.0;
	return  (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{	
	float depthValue = texture(screenTex, vTex).r;

	fColor = vec4(vec3(linerizeDepth(depthValue) / farPlane), 1.0); //perspective
	//or
	//fColor = vec4(vec3(depthValue), 1.0);				 //orthographic
	
}