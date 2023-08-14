#include "Object.h"

///////////////////////////////////////////
// Object class
//////////////////////

Object::Object()
{	
	pos = glm::vec3(0.f, 0.f, 0.f);
	angles = glm::vec3(0.f, 0.f, 0.f);
	scope = glm::vec3(1.f, 1.f, 1.f);
	model = glm::mat4(1.f);
}

Object::Object(const Model& model) : Object()
{
	this->meshes = model.meshes;
}

Object::Object(const std::vector<Mesh>& meshes, glm::vec3 pos) : Object()
{
	translate(pos);
	this->meshes = meshes;
}

Object::Object(const std::vector<Mesh>& meshes) : Object()
{	
	this->meshes = meshes;
}

Object::Object(const Mesh& mesh, glm::vec3 pos) : Object()
{
	meshes.push_back(mesh);
	translate(pos);
}

Object::Object(const Mesh& mesh) : Object()
{
	meshes.push_back(mesh);
}

glm::vec3 Object::getPos() const
{
	return pos;
}

glm::mat4 Object::getModel() const
{
	return model;
}

//////////////////////////////////
// Sets

void Object::translate(glm::vec3 coords)
{
	pos = coords;
	updateModelMatrix();
}

void Object::scale(glm::vec3 scope)
{
	this->scope *= scope;
	updateModelMatrix();
}

////////////////////////////////////
// Transformations

void Object::move() 
{
	
}

void Object::rescale(float k)
{
	scope *= k;
}

void Object::rotateX(float angle)
{	
	angles.x += angle;
	if (angles.x > 359)
	{
		angles.x = static_cast<int>(angles.x) % 360;
	} 
	else
	if (angles.x < -359)
	{
		angles.x = static_cast<int>(angles.x) % 360;
	}
	updateModelMatrix();
}

void Object::rotateY(float angle)
{
	angles.y += angle;
	if (angles.y > 359)
	{
		angles.y = static_cast<int>(angles.y) % 360;
	}
	else
		if (angles.y < -359)
		{
			angles.y = static_cast<int>(angles.y) % 360;
		}
	updateModelMatrix();
}

void Object::rotateZ(float angle)
{
	angles.z += angle;
	if (angles.z > 359)
	{
		angles.z = static_cast<int>(angles.z) % 360;
	}
	else
		if (angles.z < -359)
		{
			angles.z = (static_cast<int>(angles.z) % 360) * (-1);
		}
	updateModelMatrix();
}

void Object::updateModelMatrix()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::scale(model, scope);
	model = glm::rotate(model, glm::radians(angles.x), glm::vec3(1.0f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.f, 0.f, 1.f));
}

// Draw

void Object::draw(Shader& shader)
{
	shader.use();
	shader.setMat4("vuModel", model);

	for (auto& x : meshes)
	{
		x.draw(shader);
	}
}


