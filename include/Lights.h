#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm\glm.hpp>

////////////////////////////////
// Lights

//lamp 0.2 0.2 0.2, 0.5 0.5 0.5, 1.0 1.0 1.0
//------------------------------------------
const glm::vec3 LampAmbient(0.2f, 0.2f, 0.2f);
const glm::vec3 LampDiffuse(0.5f, 0.5f, 0.5f);
const glm::vec3 LampSpecular(1.0f, 1.0f, 1.0f);


#endif