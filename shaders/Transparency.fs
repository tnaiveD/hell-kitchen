#version 330 core

in vec2 vTex;

out vec4 fColor;

struct Material
{
	sampler2D tex_diffuse0;
};

uniform Material fuMaterial;

void main(){	
	vec4 color = texture(fuMaterial.tex_diffuse0, vTex);
	if(color.a < 0.1)
		discard;
	else
		fColor = color;
}