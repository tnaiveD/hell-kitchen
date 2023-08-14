#ifndef DEBUG_H
#define DEBUG_H

#include <glm/glm.hpp>

#include "Shader.h"
#include "Light.h"
#include "Object.h"

/////////////////////////////////////////////////////////
// <Type> to <Type> transform
//////////////////////////////


/*


*/

std::string vec3ToString(glm::vec3 vec)
{
	return std::string("[" + std::to_string(vec.x) + ", " +
		std::to_string(vec.y) + ", " +
		std::to_string(vec.z) + "]");
};



#endif // !DEBUG_H
