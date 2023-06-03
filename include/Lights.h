#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm\glm.hpp>

////////////////////////////////
// Lights

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


//lamp 0.2 0.2 0.2, 0.5 0.5 0.5, 1.0 1.0 1.0
//------------------------------------------
const glm::vec3 LampAmbient(0.02f, 0.02f, 0.02f);
const glm::vec3 LampDiffuse(0.6f, 0.6f, 0.6f);
const glm::vec3 LampSpecular(1.f, 1.f, 1.f);

const glm::vec3 LightRed(1.0f, 0.0f, 0.0f);
const glm::vec3 LightGreen(0.0f, 1.0f, 0.0f);
const glm::vec3 LightBlue(0.0f, 0.0f, 1.0f);
const glm::vec3 LightYellow(1.0f, 0.0f, 0.0f);

#endif