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

namespace mesh
{

	struct Vertex
	{
		vec3 position;
		vec2 normal;
		vec2 texCoord;
	};

	struct Texture
	{
		unsigned int id;
		string type;
	};

	//////////////////////////////////
	// Mesh

	class Mesh
	{
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		Mesh(vector<Vertex>, vector<unsigned int>, vector<Texture>);

		void draw(Shader);
	private:
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		void setupMesh();
	};

}






#endif