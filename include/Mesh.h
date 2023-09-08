#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <glm\glm.hpp>

#include "stb_image.h"
#include "Shader.h"

using std::string;
using std::vector;
using glm::vec2;
using glm::vec3;
using glm::mat4;

/////////////////////////////////////////
// Vertex
//////////////////

/*
*	Specify type of a vertex, which passed to f(<type>, Vertex)
*
*	V - vertices only.
*	V_N - vertices + normals.
*	V_T - vertices + textures.
*	V_N_T - vertices + normals + textures.
*/

enum VertexType
{
	VERTEX = 1,
	VERTEX_P_T
};

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texCoords;
};

class VertexVector
{
public:

	VertexVector(std::vector<Vertex>);
	VertexVector(std::vector<Vertex>, VertexType);

	std::vector<Vertex> getVertexVector() const;

private:

	std::vector<Vertex> vertexVec;
	VertexType vertexType;
};

/* Texture type */

enum TextureType
{
	TEXTURE_DIFFUSE = 1,
	TEXTURE_SPECULAR,
	TEXTURE_NORMALS,
	TEXTURE_HEIGHT
};

struct Texture
{
	unsigned int id;
	TextureType type;
	string path;
};

//////////////////////////////////////
// Mesh
///////////////////

class Mesh
{
public:

	vector<Vertex> vertices;
	
	// Type of vertexes stored in "vertices", see below enum "VertexType"
	VertexType verticesType;

	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	// Vertices and textures, no indices (with further glDrawArrays call)
	Mesh(vector<Vertex> vertices, vector<Texture> textures);
	Mesh(vector<Vertex> vertices, Texture texture);

	// Full-fledged vertex, drawing via glDrawElements
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	// Loading texture stuff
	void loadMeshTexture(const char* path, GLenum wrapMode, TextureType type);

	// Draw
	void draw(Shader& shader);

private:

	unsigned int VBO;
	unsigned int EBO;

	//Data configuring
	void setupMesh();
};

// Pass vertices data and one of VERTEX above to convert
vector<Vertex> vertexDataToVertexVector(const float* vertexData, int size, VertexType vertexType);

/////////////////////////////////////////
// Texture loading
/////////////////////

/*
* Pass the target path and TEXTURE WRAP mode (GL_REPEAT, GL_CLAMP_TO_EDGE, etc.)
* This fun using stbi_image
*/
unsigned int loadTex(const char* path, GLenum wrapMode);

/*
* Similar to loadTex(), but returns Texture object, not ID 
*/
Texture loadTexTex(const char* path, GLenum wrapMode, TextureType type);

/*
* Load cubemap using 6 images calling "faces" (top.<format>, bottom.<format>, etc.)
* Pass the vector which contains their names
*/
unsigned int loadCubemap(const char* dirPath, std::vector<std::string> faces);

#endif