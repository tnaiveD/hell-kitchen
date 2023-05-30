#ifndef MATERIALS_H
#define MATERIALS_H

#include <glm\glm.hpp>

/////////////////////////////////////////
// Materials

//silver 0.19225 0.19225 0.19225, 0.50754 0.50754 0.50754, 0.508273	0.508273 0.508273, 0.4
//-----------------------------------------------------
const glm::vec3 SilverAmbient(0.19225f, 0.19225f, 0.19225f);
const glm::vec3 SilverDiffuse(0.50754f, 0.50754f, 0.50754f);
const glm::vec3 SilverSpecular(0.508273f, 0.508273f, 0.508273f);
const float SilverShininess(256.0f);


#endif

