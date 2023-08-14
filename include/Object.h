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
	Object(const Model& model);
	Object(const std::vector<Mesh>& meshes, glm::vec3 pos);
	Object(const std::vector<Mesh>& meshes);
	Object(const Mesh& mesh, glm::vec3 pos);
	Object(const Mesh& mesh);

	////////////////////////////////
	// Get
	glm::vec3 getPos() const;
	glm::mat4 getModel() const;

	////////////////////////////////
	// Transforms
	void translate(glm::vec3 coords); // pos setter
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void scale(glm::vec3 scope);
	
	// ...
	void move();
	void rescale(float);
	
	////////////////////////////////
	// Draw

	void draw(Shader&);

private:
	vector<Mesh> meshes;
	glm::mat4 model;
	
	// Object actual properties
	glm::vec3 pos;
	glm::vec3 angles;
	glm::vec3 scope;

	void updateModelMatrix();
};


#endif