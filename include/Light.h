#ifndef LIGHT_H
#define LIGHT_H

#include<glm\glm.hpp>
#include "Timer.h"

/////////////////////////////////////////////
// Light properties
/////////////////////////////////////////////

// default 0.2 0.2 0.2, 0.5 0.5 0.5, 1.0 1.0 1.0
// ------------------------------------------
#define AMBIENT_DEFAULT glm::vec3(0.2f, 0.2f, 0.2f)
#define DIFFUSE_DEFAULT glm::vec3(0.5f, 0.5f, 0.5f)
#define SPECULAR_DEFAULT glm::vec3(1.0f, 1.0f, 1.0f)

//////////////////////////////////////////////////
// Attenuations
//////////////////////////////////////////////////

//		Range					  Constant  Linear   Quadratic
#define ATTENUATION_INF glm::vec3(1.0f,     0.0f,    0.0f)
#define ATTENUATION7    glm::vec3(1.0f,     0.7f,    1.8f)
#define ATTENUATION13   glm::vec3(1.0f,     0.35f,   0.44f)
#define ATTENUATION20   glm::vec3(1.0f,     0.22f,   0.20f)
#define ATTENUATION32   glm::vec3(1.0f,     0.14f,   0.07f)
#define ATTENUATION50   glm::vec3(1.0f,     0.09f,   0.032f)
#define ATTENUATION65   glm::vec3(1.0f,     0.07f,   0.017f)
#define ATTENUATION100  glm::vec3(1.0f,     0.045f,  0.0075f)
#define ATTENUATION160  glm::vec3(1.0f,     0.027f,  0.0028f)
#define ATTENUATION200  glm::vec3(1.0f,     0.022f,  0.0019f)
#define ATTENUATION325  glm::vec3(1.0f,     0.014f,  0.0007f)
#define ATTENUATION600  glm::vec3(1.0f,     0.007f,  0.0002f)
#define ATTENUATION3250 glm::vec3(1.0f,     0.0014f, 0.000007f)

/*
* ################################################
* # LIGHT CLASS						 
* ################################################
*/

/////////////////////////////////////
// Abstract parent
/////////////////////////////////////

class Light 
{
public:
	Light() : ambient(AMBIENT_DEFAULT),
			  diffuse(DIFFUSE_DEFAULT),
			  specular(SPECULAR_DEFAULT),
			  attenuation(ATTENUATION13) {};

	//Set
	void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
	void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
	void setSpecular(glm::vec3 specular) { this->specular = specular; }
	void setAttenuation(glm::vec3 attenuation) { this->attenuation = attenuation; }

	//Modify
	void multAmbient(float k) { this->ambient *= k; }
	void multDiffuse(float k) { this->diffuse *= k; }
	void multSpecular(float k) { this->specular *= k; }

	//Get
	glm::vec3 getAmbient() { return this->ambient; }
	glm::vec3 getDiffuse() { return this->diffuse; }
	glm::vec3 getSpecular() { return this->specular; }
	glm::vec3 getAttenuation() { return this->attenuation; }

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 attenuation;
};

/////////////////////////////////////
// Directional light
/////////////////////////////////////

#define DIRECTION_DEFAULT glm::vec3(-1.0f, -3.0f, 0.5f)

class DirectionalLight : public Light
{
public:

	//Direction parameter
	DirectionalLight(glm::vec3 _dir = DIRECTION_DEFAULT) : dir(_dir) {};

	//Set
	void setDir(glm::vec3 dir) { this->dir = dir; }

	//Get
	glm::vec3 getDir() const { return this->dir; }

private:
	glm::vec3 dir;
};

/////////////////////////////////////
// Point light
/////////////////////////////////////

class PointLight : public Light
{
public:

	//Position parameter
	PointLight(glm::vec3 _pos = glm::vec3(0.f, 0.f, 0.f)) : pos(_pos) {};

	//Set
	void setPos(glm::vec3 pos) { this->pos = pos; }
	
	//Get
	glm::vec3 getPos() const { return this->pos; }
	
private:
	glm::vec3 pos;
};

/////////////////////////////////////
// Spotlight
/////////////////////////////////////

class SpotLight : public Light
{
public:

	//Position and Direction parameters
	SpotLight(glm::vec3 pos, glm::vec3 dir) :
		outAngle(34.f),
		inAngle(30.f)
	{
		this->pos.x = pos.x;
		this->pos.y = pos.y;
		this->pos.z = pos.z;
		this->dir.x = dir.x;
		this->dir.y = dir.y;
		this->dir.z = dir.z;
	};

	/*SpotLight(float _posX, float _posY, float _posZ, float _dirX, float _dirY, float _dirZ)
	{

	}*/

	//Set
	void setPos(glm::vec3 pos) { this->pos = pos; }
	void setDir(glm::vec3 dir) { this->dir = dir; }
	void setAngles(float outAngle, float inAngle) { this->outAngle = outAngle; this->inAngle = inAngle; }

	//Get
	glm::vec3 getPos() const { return this->pos; }
	glm::vec3 getDir() const { return this->dir; }
	float getOutAngle() const { return this->outAngle; }
	float getInAngle() const { return this->inAngle; }
	
	//Other
	void movePos(glm::vec3);
	void moveDir(glm::vec3);

private:
	glm::vec3 pos;
	glm::vec3 dir;
	float outAngle;
	float inAngle;
};

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

class FlashLight final : public SpotLight
{
public:
	//in process...
private:
	Timer timer;
};

/////////////////////////////////////////
// Colors
/////////////////////////////////////////

//colors
//------------------------------
#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define YELLOW 1.0f, 1.0f, 0.0f
#define PURPLE 1.0f, 0.0f, 1.0f
#define CYAN 0.0f, 1.0f, 1.0f
#define BLACK 0.0f, 0.0f, 0.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define GREY .3f, .3f, .3f
#define GREY_DARK .1f, .1f, .1f
#define GREY_DARK_WARM .12f, .1f, .1f
#define GREY_DARK_COLD .1f, .11f, .125f
#define GREY_DARK_DARK .05f, .05f, .05f
#define ORANGE 0.9f, 0.5f, 0.f

//nature colors
//-------------------------------
#define SKY 0.2f, 0.5f, 0.9f



#endif