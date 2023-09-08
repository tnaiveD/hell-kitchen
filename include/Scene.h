#ifndef SCENE_H
#define SCENE_H

#include <unordered_set>
#include <vector>

#include "Object.h"
#include "Shader.h"

class Scene
{
public:
	
	Scene() : pos(1.0f) {};
	
	void add(Object* object);

	void draw(const Shader& shader);

private:
	
	std::unordered_set<Object*> objects;

	/* Scene parameters */
	glm::vec3 pos;

};


#endif