#include "Object.h"

///////////////////////////////////////////
// Object class
//////////////////////

Object::Object()
{	
	model = glm::mat4(1.0f);
	pos = glm::vec3(0.f, 0.f, 0.f);
}

Object::Object(std::vector<Mesh>& meshes, glm::vec3 pos) : Object()
{
	moveTo(pos);
	this->meshes = meshes;
}

Object::Object(std::vector<Mesh>& meshes) : Object()
{	
	this->meshes = meshes;
}

Object::Object(Mesh& mesh, glm::vec3 pos) : Object()
{
	moveTo(pos);
	meshes.push_back(mesh);
}

Object::Object(Mesh& mesh) : Object()
{
	meshes.push_back(mesh);
}

glm::vec3 Object::getPos() const
{
	return this->pos;
}

glm::mat4 Object::getModel() const
{
	return model;
}

//////////////////////////////////////////
// Transforms
////////////////////

void Object::moveTo(glm::vec3 coords)
{
	model = glm::translate(model, coords);
	pos = coords;
}

void Object::rotate(float angle, glm::vec3 axes)
{
	model = glm::rotate(model, glm::radians(angle), axes);
}

void Object::scale(glm::vec3 scope)
{
	model = glm::scale(model, scope);
}

void Object::flipVertical()
{
	scale(glm::vec3(1.f, -1.f, 1.f));
}


//////////////////////////////////////////
// Draw
///////////////////
void Object::draw(Shader& shader)
{
	shader.use();
	shader.setMat4("vuModel", model);
	for (auto& x : meshes)
	{
		x.draw(shader);
	}
}



