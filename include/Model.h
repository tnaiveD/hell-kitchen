#ifndef MODEL_H
#define MODEL_H

//OS specify
#ifndef WIN32
#define WIN32



#elif LINUX
#define

#endif


#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "stb_image.h"

using std::string;
namespace fs = std::filesystem;


unsigned int texFromFile(const char*, const string&);

class Model
{
public:
	Model(const string&, bool);

	void draw(Shader&);

private:
	std::vector<Mesh> meshes;
	string directory;
	vector<Texture> texLoaded;
	bool gammaCorrection;

	void loadModel(string);

	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	vector<Texture> loadTextures(aiMaterial*, aiTextureType, string);
};


#endif
