#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "stb_image.h"

using std::string;

unsigned int texFromFile(const char*, const string&);

class Model
{
public:
	Model(const char*);

	void draw(Shader&);




private:
	std::vector<Mesh> meshes;
	string directory;
	vector<Texture> texLoaded;

	void loadModel(string);

	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, string);
};


#endif
