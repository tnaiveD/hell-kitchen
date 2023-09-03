#version 420 core

/////////////////////////////////////
// Out
//////////////////

out vec4 fColor;

/////////////////////////////////////
// In
///////////////////

in VS_OUT
{
	vec3 pos;
	vec3 normal;
	vec2 tex;
	vec4 lightSpacePos;
} fs_in;

/////////////////////////////////////
// Declarations
///////////////////

struct Material
{
	sampler2D tex_ambient0;

	sampler2D tex_diffuse0;
	sampler2D tex_diffuse1;

	sampler2D tex_specular0;

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

// Funs
// ------------------------------------

vec3 calcDirLight(DirLight, vec3);

vec3 calcPointLight(PointLight, vec3);

vec3 calcSpotLight(SpotLight, vec3);

float calcShadow(vec4, vec3);

////////////////////////////////////////
// Data init.
////////////////////

// Material
uniform Material fuMaterial;

// Lights
// -------------------------------------

uniform DirLight DLight;

#define MAX_PLIGHTS 16
uniform PointLight PLight[MAX_PLIGHTS];
uniform int PLnum;

#define MAX_SLIGHTS 16
uniform SpotLight SLight[MAX_SLIGHTS];
uniform int SLnum;

// Shadows
// --------------------------------------

uniform sampler2D shadowMap;

// Camera
// --------------------------------------

uniform vec3 fuViewPos;


//////////////////////////////////////////
// MAIN
/////////////////////

void main()
{	
	vec3 color = vec3(0.0);

	//data
	vec3 viewDir = normalize(fuViewPos - fs_in.pos);

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
		color += DLight.ambient * vec3(texture(fuMaterial.tex_diffuse0, fs_in.tex));

	//////////////////////////////////////////////
	
	fColor = vec4(color, 1.0);
	
	//or gamma corrected
	//float gamma = 2.2;
	//fColor.rgb = pow(color.rgb, vec3(1.0/gamma));
}

vec3 calcDirLight(DirLight light, vec3 viewDir)
{
	vec3 ambient = light.ambient * vec3(texture(fuMaterial.tex_diffuse0, fs_in.tex));

	vec3 norm = normalize(fs_in.normal);
	vec3 lightDir = normalize(light.dir);
	float diff = max(0.0, dot(-lightDir, norm));
	vec3 diffuse = light.diffuse * diff * vec3(texture(fuMaterial.tex_diffuse0, fs_in.tex));

	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(0.0, dot(viewDir, reflectDir)), fuMaterial.shininess);
	vec3 specular = light.specular * spec * vec3(texture(fuMaterial.tex_specular0, fs_in.tex));
		
	float shadow = 1 - calcShadow(fs_in.lightSpacePos, lightDir);

	return ambient + (diffuse + specular) * shadow;
}

vec3 calcPointLight(PointLight light, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - fs_in.pos);
	vec3 norm = normalize(fs_in.normal);

	vec3 ambient = light.ambient * vec3(texture(fuMaterial.tex_diffuse0, fs_in.tex));

	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = light.diffuse * diff * vec3(texture(fuMaterial.tex_diffuse0, fs_in.tex));

	//vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(0.0, dot(halfDir, norm)), fuMaterial.shininess);
	vec3 specular = light.specular * spec * vec3(texture(fuMaterial.tex_specular0, fs_in.tex));

	float d = length(light.pos - fs_in.pos);
	float attenuation = 1 / (light.constant + light.linear * d + light.quadratic * (d * d));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + (diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - fs_in.pos);
	float theta = dot(normalize(-light.dir), lightDir);
		
	if(theta > light.outAngle)
	{
		
		vec3 norm = normalize(fs_in.normal);

		vec3 ambient = light.ambient * vec3(texture(fuMaterial.tex_diffuse0, fs_in.tex));

		float diff = max(0.0, dot(norm, lightDir));
		vec3 diffuse = light.diffuse * diff * vec3(texture(fuMaterial.tex_diffuse0, fs_in.tex));

		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(0.0, dot(viewDir, reflectDir)), fuMaterial.shininess);
		vec3 specular = light.specular * spec * vec3(texture(fuMaterial.tex_specular0, fs_in.tex));

		float e = light.inAngle - light.outAngle;
		
		//float intensity = clamp((theta - light.outAngle) / e, 0.0, 1.0);
		//or
		float intensity = smoothstep(0.0, 1.0, (theta - light.outAngle) / e);

		diffuse *= intensity;
		specular *= intensity;

		float d = length(light.pos - fs_in.pos);
		float attenuation = 1 / (light.constant + light.linear * d + light.quadratic * (d * d));
	
		diffuse *= attenuation;
		specular *= attenuation;
		
		float shadow = 1 - calcShadow(fs_in.lightSpacePos, lightDir);

		return (diffuse + specular) * (1 - shadow);
	}
	else
	{
		return light.ambient * vec3(texture(fuMaterial.tex_diffuse0, fs_in.tex));	
	}
}

float calcShadow(vec4 lightSpacePos, vec3 lightDir)
{	
	vec3 projectionPos = lightSpacePos.xyz / lightSpacePos.w;
	if(projectionPos.z > 1.0)
		return 0.0;
	projectionPos = projectionPos * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projectionPos.xy).r;
	float currentDepth = projectionPos.z;

	//float bias = max(0.00492 * (1.0 - dot(fs_in.normal, lightDir)), 0.004);
	float bias = 0.00195;
	
	float shadow = 0.0;
	vec2 texelSize = 2.0 / textureSize(shadowMap, 0);
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			float pcfDepth = texture(shadowMap, projectionPos.xy + vec2(i, j) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	
	shadow /= 9; // 9
	
	return shadow;
}