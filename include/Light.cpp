#include "Light.h"

///////////////////////////////////////////
// Abstract parent
////////////////////////

Light::Light() : ambient(AMBIENT_DEFAULT),
		diffuse(DIFFUSE_DEFAULT),
		specular(SPECULAR_DEFAULT),
		attenuation(ATTENUATION_INF),
		active(true) {};

//Set
void Light::setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
void Light::setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
void Light::setSpecular(glm::vec3 specular) { this->specular = specular; }
void Light::setAttenuation(glm::vec3 attenuation) { this->attenuation = attenuation; }
void Light::setActive(bool state) { this->active = state; }

//Modify
void Light::multAmbient(float k) { this->ambient *= k; }
void Light::multDiffuse(float k) { this->diffuse *= k; }
void Light::multSpecular(float k) { this->specular *= k; }

//Get
glm::vec3 Light::getAmbient() const { return this->ambient; }
glm::vec3 Light::getDiffuse() const { return this->diffuse; }
glm::vec3 Light::getSpecular() const { return this->specular; }
glm::vec3 Light::getAttenuation() const { return this->attenuation; }
bool Light::isActive() const { return this->active; }

////////////////////////////////////////////
// Directional light
//////////////////////

//Direction parameter
DirectionalLight::DirectionalLight(glm::vec3 _dir) : dir(_dir) {};

//Set
void DirectionalLight::setDir(glm::vec3 dir) { this->dir = dir; }

//Get
glm::vec3 DirectionalLight::getDir() const { return this->dir; }

///////////////////////////////////////////
// Point light
///////////////////////

unsigned PointLight::count = 0;

unsigned PointLight::getCount() { return count; }

//Position parameter
PointLight::PointLight(glm::vec3 _pos) : pos(_pos)
{ 
	id = count;
	count++; 
};

PointLight::PointLight(float x, float y, float z)
{
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;

	id = count;
	count++;
};


//Set
void PointLight::setPos(glm::vec3 pos) { this->pos = pos; }

void PointLight::setPos(float x, float y, float z)
{
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
}

//Get
glm::vec3 PointLight::getPos() const { return this->pos; }

unsigned PointLight::getId() const { return this->id; }

/////////////////////////////////////
// Spotlight
/////////////////////////////////////

unsigned SpotLight::count = 0;

unsigned SpotLight::getCount() { return count; }

//Position and Direction parameters
SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir)
{
	this->pos = pos;
	this->dir = dir;

	setRadius(36.f);

	id = count;
	count++;
};

SpotLight::SpotLight(float posX, float posY, float posZ, float dirX, float dirY, float dirZ)
{
	this->pos.x = posX;
	this->pos.y = posY;
	this->pos.z = posZ;

	this->dir.x = dirX;
	this->dir.y = dirY;
	this->dir.z = dirZ;

	setRadius(36.f);

	id = count;
	count++;
}
//Set
void SpotLight::setPos(glm::vec3 pos) { this->pos = pos; }
void SpotLight::setPos(float x, float y, float z)
{
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
}

void SpotLight::setDir(glm::vec3 dir) { this->dir = dir; }
void SpotLight::setAngles(float outAngle, float inAngle) { this->outAngle = outAngle; this->inAngle = inAngle; }
void SpotLight::setRadius(float radius)
{
	setAngles(radius, radius * 0.7f);
}

//Get
glm::vec3 SpotLight::getPos() const { return this->pos; }
glm::vec3 SpotLight::getDir() const { return this->dir; }
float SpotLight::getOutAngle() const { return this->outAngle; }
float SpotLight::getInAngle() const { return this->inAngle; }
unsigned SpotLight::getId() const { return this->id; }


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


// Other
// --------------------------------------
