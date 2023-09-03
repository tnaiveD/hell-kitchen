#include "Scene.h"

void Scene::add(Object* object)
{
	//objects.insert(object);
}
 
void Scene::draw(const Shader& shader)
{
	for (const auto& x : objects)
	{
		x->draw(shader);
	}
}