#ifndef LIGHT_H
#define LIGHT_H

#include <glm\glm.hpp>
#include <glm\vector_relational.hpp>

#include <vector>
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

/////////////////////////////////////
// 

struct ShadowMap;

/*
* ################################################
* #				   LIGHT CLASS					 #						 
* ################################################
*/

///////////////////////////////////////////////////
// Abstract parent
//////////////////////////

class Light 
{
public:
	
	Light();
	virtual ~Light() = default;

	//Set
	void setAmbient(glm::vec3 color);
	void setDiffuse(glm::vec3 color);
	void setSpecular(glm::vec3 color);
	void setAttenuation(glm::vec3 const_linear_quadr);
	void setActive(bool state);

	//Modify
	void multAmbient(float k);
	void multDiffuse(float k);
	void multSpecular(float k);

	void addToAmbient(float k);
	void addToDiffuse(float k);
	void addToSpecular(float k);

	//Get
	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;
	glm::vec3 getAttenuation() const;
	bool isActive() const;

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 attenuation;
	bool active;
};

/////////////////////////////////////////////////////
// Directional light
//////////////////////////

class DirectionalLight : public Light
{
public:

	//Direction parameter
	DirectionalLight(glm::vec3 = glm::vec3(-1.0f, -3.0f, 0.25f));

	//Set
	void setDir(glm::vec3 dir);

	//Get
	glm::vec3 getDir() const;

private:
	glm::vec3 dir;
};

////////////////////////////////////////////////////////
// Point light
////////////////////////////

class PointLight : public Light
{
public:

	//Position parameter
	PointLight(glm::vec3 pos = glm::vec3(0.f, 1.5f, 0.f));
	PointLight(float posX, float posY, float posZ);

	//Set
	void setPos(glm::vec3 pos);
	void setPos(float posX, float posY, float posZ);

	//Get
	glm::vec3 getPos() const;
	unsigned getId() const;
	static unsigned getCount();

private:
	glm::vec3 pos;

	unsigned id;

	static unsigned count;
};


////////////////////////////////////////////////////
// Spotlight
//////////////////////////

class SpotLight : public Light
{
public:
	//Position and Direction parameters
	SpotLight(glm::vec3 pos = glm::vec3(0.f, 2.f, 0.f), glm::vec3 dir = glm::vec3(0.f, -1.f, 0.f));
	SpotLight(float posX, float posY, float posZ, float dirX, float dirY, float dirZ);
	
	//Set
	void setPos(glm::vec3 pos);
	void setPos(float posX, float posY, float posZ);
	void setDir(glm::vec3 dir);
	void setAngles(float outAngle, float inAngle);
	void setRadius(float radius);

	//Get
	glm::vec3 getPos() const;
	glm::vec3 getDir() const;
	float getOutAngle() const;
	float getInAngle() const;
	unsigned getId() const;

	static unsigned getCount();

	//Other
	void movePos(glm::vec3 pos);
	void moveDir(glm::vec3 dir);

private:

	glm::vec3 pos;
	glm::vec3 dir;
	float outAngle;
	float inAngle;

	unsigned id;

	static unsigned count;
};


// Flashlight
// --------------------------------------

class FlashLight final : public SpotLight
{
public:
	//in dev...
private:
	Timer timer;
};

//////////////////////////////////////////////
// Shadows
///////////////////////

struct ShadowMap
{
	unsigned int textureID;
};

//////////////////////////////////////////////
// Colors
///////////////////////

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