/*
  
  #############################################
  
  Tools for work with Models and Meshes:

  - Object class

  #############################################

*/

#ifndef OBJECT_H
#define OBJECT_H

#include <ostream>
#include <glm\glm.hpp>

#include "stb_image.h"
#include "Model.h"
#include "Shader.h"

class Object
{
public:

	Object();
	Object(std::vector<Mesh>& meshes, glm::vec3 pos);
	Object(std::vector<Mesh>& meshes);
	Object(Mesh& meshes, glm::vec3 pos);
	Object(Mesh& meshes);

	// Get
	glm::vec3 getPos() const;
	glm::mat4 getModel() const;

	// Draw
	void draw(Shader&);

	// Transforms
	void moveTo(glm::vec3 coords);
	void rotate(float angle, glm::vec3 axes);
	void scale(glm::vec3 scope);
	void flipVertical();

private:
	vector<Mesh> meshes;
	glm::mat4 model;
	glm::vec3 pos;
};


#endif