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
*	Vertex type... f(<type>, Vertex)
*	
*	VERTEX - pos + normals + textures.
*	VERTEX_P - pos only.
*	VERTEX_P_N - pos + normals.
*	VERTEX_P_T - pos + textures.
*	
*/

enum VertexType
{
	VERTEX = 1,
	VERTEX_P,
	VERTEX_P_N,
	VERTEX_P_T
};

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texCoords;

	vec3 tangent;
	vec3 bitangent;
	vector<unsigned> boneIDs;
	vector<float> weights;
};

class VertexVector
{
public:

	VertexVector();
	VertexVector(const std::vector<Vertex>& verticesVec, VertexType vertexType = VERTEX);

	std::vector<Vertex> getVertexVector() const;
	VertexType getVertexType() const;

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
	string path;
	TextureType type;

};

/////////////////////////////////////////
// Materials
/////////////////////

struct Material
{
	std::string name;
	float shininess;
	Material();
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

	VertexVector vertexVec;

	vector<unsigned int> indices;
	vector<Texture> textures;
	Material material;

	unsigned int VAO;

	// Full-fledged vertex, drawing via glDrawElements
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	// Vertices and textures, no indices (with further glDrawArrays call)
	Mesh(const VertexVector& vertexVec, vector<Texture> textures);
	Mesh(const VertexVector& vertexVec, Texture texture);
	
	// Single vertex vector
	Mesh(const VertexVector& vertexVec);


	// Loading texture stuff
	void loadMeshTexture(const char* path, GLenum wrapMode, TextureType type);

	// Draw
	void draw(const Shader& shader) const;

private:

	unsigned int VBO;
	unsigned int EBO;

	//Data configuring
	void setupMesh();
};


/////////////////////////////////////////
// Other
/////////////////////

// Pass vertices data and one of VERTEX above to convert
vector<Vertex> vertexDataToVertexVector(const float* vertexData, int size, VertexType vertexType);

// Reset samplers2d (by default - GL_TEXTURE(0...6)
void resetSamplers2D();

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