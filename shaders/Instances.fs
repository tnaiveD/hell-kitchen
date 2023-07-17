#version 420 core

in vec2 vTex;

out vec4 fColor;

struct Material
{
	sampler2D tex_ambient0;

	sampler2D tex_diffuse0;

	sampler2D tex_specular0;
};

uniform Material material;

void main()
{
	fColor = texture(material.tex_diffuse0, vTex);
}