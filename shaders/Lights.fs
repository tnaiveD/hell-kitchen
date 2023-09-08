#version 420 core

out vec4 fColor;

in vec3 vPos;
in vec3 vNormals;
in vec2 vTex;

in VS_OUT
{
	vec2 texCoord;

} fs_in;

/////////////////////////////

struct Material
{
	sampler2D tex_ambient0;

	sampler2D tex_diffuse0;
	sampler2D tex_diffuse1;

	sampler2D tex_specular0;

	sampler2D tex_normal0;

	float shininess;
};

struct DirLight
{
	bool active;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 dir;
};

struct PointLight
{
	bool active;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 pos;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	bool active;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 pos;
	vec3 dir;

	float constant;
	float linear;
	float quadratic;

	float outAngle;
	float inAngle;
};

// MATERIAL
// --------------------------------------
uniform Material fuMaterial;

// LIGHTS
// --------------------------------------

uniform DirLight DLight;

#define MAX_PLIGHTS 16
uniform PointLight PLight[MAX_PLIGHTS];
uniform int PLnum;

#define MAX_SLIGHTS 16
uniform SpotLight SLight[MAX_SLIGHTS];
uniform int SLnum;

vec3 calcDirLight(DirLight, vec3);
vec3 calcPointLight(PointLight, vec3);
vec3 calcSpotLight(SpotLight, vec3);

// CAMERA
// --------------------------------------

uniform vec3 fuViewPos;


/////////////////////////////////////////
// MAIN

void main()
{	
	vec3 color = vec3(0.0);

	//data
	vec3 viewDir = normalize(fuViewPos - vPos);


	//directional light
	if(DLight.active)
	{
		color += calcDirLight(DLight, viewDir);
	}

	//point lights
	bool PLCheckActive = false;
	for(int i = 0; i < PLnum; i++)
	{
		if(PLight[i].active)
		{
			color += calcPointLight(PLight[i], viewDir);
			if(!PLCheckActive) PLCheckActive = true;
		}
	}

	//spotlights
	bool SLCheckActive = false;
	for(int i = 0; i < SLnum; i++)
	{	
		if(SLight[i].active) 
		{
			color += calcSpotLight(SLight[i], viewDir);
			if(!SLCheckActive) SLCheckActive = true;
		}
	}
	
	
	if ((!DLight.active) && (!SLCheckActive) && (!PLCheckActive))
		color += vec3(texture(fuMaterial.tex_diffuse0, vTex));

	//////////////////////////////////////////////
	
	fColor = vec4(color, 1.0);
	
	//or gamma corrected
	//float gamma = 2.2;
	//fColor.rgb = pow(color.rgb, vec3(1.0/gamma));
}

vec3 calcDirLight(DirLight light, vec3 viewDir)
{
	vec3 ambient = light.ambient * vec3(texture(fuMaterial.tex_diffuse0, vTex));

	vec3 norm = normalize(vNormals);
	vec3 lightDir = normalize(light.dir);
	float diff = max(0.0, dot(-lightDir, norm));
	vec3 diffuse = light.diffuse * diff * vec3(texture(fuMaterial.tex_diffuse0, vTex));

	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(0.0, dot(viewDir, reflectDir)), fuMaterial.shininess);
	vec3 specular = light.specular * spec * vec3(texture(fuMaterial.tex_specular0, vTex));
		
	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - vPos);
	vec3 norm = normalize(vNormals);

	vec3 ambient = light.ambient * vec3(texture(fuMaterial.tex_diffuse0, vTex));

	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = light.diffuse * diff * vec3(texture(fuMaterial.tex_diffuse0, vTex));

	//vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(0.0, dot(halfDir, norm)), fuMaterial.shininess);
	vec3 specular = light.specular * spec * vec3(texture(fuMaterial.tex_specular0, vTex));

	float d = length(light.pos - vPos);
	float attenuation = 1 / (light.constant + light.linear * d + light.quadratic * (d * d));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - vPos);
	float theta = dot(normalize(-light.dir), lightDir);
		
	if(theta > light.outAngle)
	{
		
		vec3 norm = normalize(vNormals);

		vec3 ambient = light.ambient * vec3(texture(fuMaterial.tex_diffuse0, vTex));

		float diff = max(0.0, dot(norm, lightDir));
		vec3 diffuse = light.diffuse * diff * vec3(texture(fuMaterial.tex_diffuse0, vTex));

		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(0.0, dot(viewDir, reflectDir)), fuMaterial.shininess);
		vec3 specular = light.specular * spec * vec3(texture(fuMaterial.tex_specular0, vTex));

		float e = light.inAngle - light.outAngle;
		
		//float intensity = clamp((theta - light.outAngle) / e, 0.0, 1.0);
		//or
		float intensity = smoothstep(0.0, 1.0, (theta - light.outAngle) / e);

		diffuse *= intensity;
		specular *= intensity;

		float d = length(light.pos - vPos);
		float attenuation = 1 / (light.constant + light.linear * d + light.quadratic * (d * d));
	
		diffuse *= attenuation;
		specular *= attenuation;
		
		return diffuse + specular;
	}
	else
	{
		return light.ambient * vec3(texture(fuMaterial.tex_diffuse0, vTex));	
	}

}