#include "Model.h"

Model::Model(const string& path, bool gamma = false) : gammaCorrection(gamma)
{
	loadModel(path);
}

void Model::draw(Shader& shader)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}
}

void Model::loadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate);
	
	/*
	* - aiProcess_FlipUVs: set vertically flip UV y-axis.
	* - aiProcess_GenNormals: creates normal vectors for each vertex if the model doesn't contain normal vectors.
	* - aiProcess_SplitLargeMeshes: splits large meshes into smaller sub-meshes which is useful if your rendering has a 
	*  							    maximum number of vertices allowed and can only process smaller meshes.
	* - aiProcess_OptimizeMeshes: does the reverse by trying to join several meshes into one larger mesh, reducing drawing calls for optimization.
	*/

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error: MODEL_H. " << importer.GetErrorString() << '\n';
		return;
	}

	directory = path.substr(0, path.find_last_of('\\'));
	if (directory == path)
	{
		directory = path.substr(0, path.find_last_of('/'));
	}
	if (directory == path)
	{
		std::cout << "Warning: MODEL_H. Directory not specified correctly \"" << directory << "\"\n";
	}

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//process all the node's meshes
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	//do the same for each of its childrens
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	//retrieve vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		
		vec3 vertVec;
		vertVec.x = mesh->mVertices[i].x;
		vertVec.y = mesh->mVertices[i].y;
		vertVec.z = mesh->mVertices[i].z;
		vertex.position = vertVec;

		vertVec.x = mesh->mNormals[i].x;
		vertVec.y = mesh->mNormals[i].y;
		vertVec.z = mesh->mNormals[i].z;
		vertex.normal = vertVec;

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
			vertex.texCoords = vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	//indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "tex_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "tex_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "tex_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "tex_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;
		
		for (int j = 0; j < texLoaded.size(); j++)
		{
			if (std::strcmp(str.C_Str(), texLoaded[j].path.c_str()) == 0)
			{
				skip = true;
				textures.push_back(texLoaded[j]);
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = texFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			texLoaded.push_back(texture);
		}
	}

	return textures;
}

unsigned int texFromFile(const char* path, const string& directory)
{
	string full_path = directory + '\\' + path;
	unsigned int id;
	glGenTextures(1, &id);
	int imgW, imgH, imgCh;
	unsigned char* imgData = stbi_load(full_path.c_str(), &imgW, &imgH, &imgCh, 0);
	stbi_set_flip_vertically_on_load(true);
	if (imgData) {
		GLenum format = 0;
		if (imgCh == 1) format = GL_RED;
		if (imgCh == 3) format = GL_RGB;
		if (imgCh == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, imgW, imgH, 0, format, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //GL_LINEAR_MIPMAP_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_LINEAR
	}
	else
		std::cout << "Warning: MODEL_H. Texture not loaded\n";
	stbi_image_free(imgData);

	return id;
}