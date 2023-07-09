#include "Light.h"

/////////////////////////////////////
// Abstract parent
/////////////////////////////////////

Light::Light() : ambient(AMBIENT_DEFAULT),
		diffuse(DIFFUSE_DEFAULT),
		specular(SPECULAR_DEFAULT),
		attenuation(ATTENUATION_INF) {};

	//Set
void Light::setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
void Light::setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
void Light::setSpecular(glm::vec3 specular) { this->specular = specular; }
void Light::setAttenuation(glm::vec3 attenuation) { this->attenuation = attenuation; }

	//Modify
void Light::multAmbient(float k) { this->ambient *= k; }
void Light::multDiffuse(float k) { this->diffuse *= k; }
void Light::multSpecular(float k) { this->specular *= k; }

	//Get
glm::vec3 Light::getAmbient() const { return this->ambient; }
glm::vec3 Light::getDiffuse() const { return this->diffuse; }
glm::vec3 Light::getSpecular() const { return this->specular; }
glm::vec3 Light::getAttenuation() const { return this->attenuation; }

/////////////////////////////////////
// Directional light
/////////////////////////////////////

DirectionalLight::DirectionalLight() : dir(DIRECTION_DEFAULT) {};

	//Direction parameter
DirectionalLight::DirectionalLight(glm::vec3 _dir = DIRECTION_DEFAULT) : dir(_dir) {};

	//Set
void DirectionalLight::setDir(glm::vec3 dir) { this->dir = dir; }

	//Get
glm::vec3 DirectionalLight::getDir() const { return this->dir; }

/////////////////////////////////////
// Point light
/////////////////////////////////////

	//Position parameter
PointLight::PointLight(glm::vec3 _pos = glm::vec3(0.f, 0.f, 0.f)) : pos(_pos) {};

	//Set
void PointLight::setPos(glm::vec3 pos) { this->pos = pos; }

	//Get
glm::vec3 PointLight::getPos() const { return this->pos; }

/////////////////////////////////////
// Spotlight
/////////////////////////////////////


	//Position and Direction parameters
SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir) : outAngle(34.f), inAngle(30.f)
	{
		this->pos = pos;
		this->dir = dir;
	};

	/*SpotLight(float _posX, float _posY, float _posZ, float _dirX, float _dirY, float _dirZ)
	{

	}*/

	//Set
void SpotLight::setPos(glm::vec3 pos) { this->pos = pos; }
void SpotLight::setDir(glm::vec3 dir) { this->dir = dir; }
void SpotLight::setAngles(float outAngle, float inAngle) { this->outAngle = outAngle; this->inAngle = inAngle; }

	//Get
glm::vec3 SpotLight::getPos() const { return this->pos; }
glm::vec3 SpotLight::getDir() const { return this->dir; }
float SpotLight::getOutAngle() const { return this->outAngle; }
float SpotLight::getInAngle() const { return this->inAngle; }

void SpotLight::movePos(glm::vec3 move)
{
	this->pos += move;
}

void SpotLight::moveDir(glm::vec3 move)
{
	this->dir += move;
}

// Flashlight
// --------------------------------------



