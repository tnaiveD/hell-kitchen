#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <glm\glm.hpp>

#include "Shader.h"

using std::string;
using std::vector;
using glm::vec2;
using glm::vec3;
using glm::mat4;

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	vec3 position;
	vec2 normal;
	vec2 texCoords;
	vec3 tangent;
	vec3 bitangent;
	int boneIDs[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
	unsigned int id;
	string type;
	string path;
};

//////////////////////////////////
// Mesh

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	Mesh(vector<Vertex>, vector<unsigned int>, vector<Texture>);

	void draw(Shader&);

private:
	unsigned int VBO;
	unsigned int EBO;

	void setupMesh();
};


#endif