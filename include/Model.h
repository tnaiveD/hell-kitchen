#ifndef MODEL_H
#define MODEL_H

#include <ostream>
#include <filesystem>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"

using std::string;
namespace fs = std::filesystem;


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
	vector<Texture> loadTextures(aiMaterial*, aiTextureType, TextureType);
};

unsigned int texFromFile(const char*, const string&);

#endif
