#version 330 core

in vec3 vPos;
in vec3 vNormal;

out vec4 fColor;

uniform vec3 fuCamPos;
uniform samplerCube fuCubemap;

void main(){
	vec3 camDir = normalize(vPos - fuCamPos);
	vec3 norm = normalize(vNormal);
	vec3 reflectDir = reflect(camDir, norm);

	fColor = vec4(texture(fuCubemap, reflectDir).rgb, 1.0);
}