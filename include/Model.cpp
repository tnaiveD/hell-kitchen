#include "Model.h"

Model::Model(const string& path, bool gamma = false) : gammaCorrection(gamma)
{
	loadModel(path);
}

void Model::draw(const Shader& shader)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}
}

void Model::loadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	
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
	VertexType vertexType;

	bool normalsCheck = false;
	bool textureCoordsCheck = false;

	//retrieve vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		
		vec3 vertVec;
		vertVec.x = mesh->mVertices[i].x;
		vertVec.y = mesh->mVertices[i].y;
		vertVec.z = mesh->mVertices[i].z;
		vertex.position = vertVec;

		if (mesh->HasNormals())
		{
			vertVec.x = mesh->mNormals[i].x;
			vertVec.y = mesh->mNormals[i].y;
			vertVec.z = mesh->mNormals[i].z;
			vertex.normal = vertVec;
			normalsCheck = true;
		}

		if (mesh->mTextureCoords[0])
		{
			vec2 texVec;
			texVec.x = mesh->mTextureCoords[0][i].x;
			texVec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = texVec;
			textureCoordsCheck = true;
		}
		else
			vertex.texCoords = vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	if ((normalsCheck) && (textureCoordsCheck))
		vertexType = VERTEX;
	else
		if (normalsCheck)
			vertexType = VERTEX_P_N;
		else
			if (textureCoordsCheck)
				vertexType = VERTEX_P_T;
			else
				vertexType = VERTEX_P;



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
		vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR, TEXTURE_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		vector<Texture> normalMaps = loadTextures(material, aiTextureType_NORMALS, TEXTURE_NORMALS);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		vector<Texture> heightMaps = loadTextures(material, aiTextureType_HEIGHT, TEXTURE_HEIGHT);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	
	Mesh res(vertices, indices, textures);
	res.verticesType = vertexType;

	return res;
}

vector<Texture> Model::loadTextures(aiMaterial* material, aiTextureType type, TextureType typeName)
{
	vector<Texture> textures;
	if (material->GetTextureCount(type) > 0)
	{
		for (unsigned int i = 0; i < material->GetTextureCount(type)/* returning 0 when mtl not used ??? */; i++)
		{
			aiString texName;
			material->GetTexture(type, i, &texName);
			bool skip = false;

			for (int j = 0; j < texLoaded.size(); j++)
			{
				if (std::strcmp(texName.C_Str(), texLoaded[j].path.c_str()) == 0)
				{
					skip = true;
					textures.push_back(texLoaded[j]);
					break;
				}
			}
			if (!skip)
			{
				Texture texture;
				texture.id = texFromFile(texName.C_Str(), directory);
				texture.type = typeName;
				texture.path = texName.C_Str();
				textures.push_back(texture);
				texLoaded.push_back(texture);
			}
		}
	}
	else
	{	/* No MTL. Use std::filesystem, search for each file in directory */
		for (const auto& _file : fs::directory_iterator(directory))
		{
			string filename = _file.path().filename().string();
			if (type == aiTextureType_DIFFUSE)
			{
				Texture texture;

				if ((filename.find("diffuse") != string::npos) || (filename.find("diff") != string::npos)) 
				{	
					texture.id = texFromFile(filename.c_str(), directory);
					texture.type = typeName;
					texture.path = _file.path().string();
					textures.push_back(texture);
					texLoaded.push_back(texture);
				} 
			} 
			else
				if (type == aiTextureType_SPECULAR)
				{
					Texture texture;

					if ((filename.find("specular") != string::npos) || (filename.find("spec") != string::npos))
					{
						texture.id = texFromFile(filename.c_str(), directory);
						texture.type = typeName;
						texture.path = _file.path().string();
						textures.push_back(texture);
						texLoaded.push_back(texture);
					}
				}
				else
					if (type == aiTextureType_NORMALS)
					{
						Texture texture;

						if ((filename.find("normal") != string::npos) || (filename.find("norm") != string::npos))
						{
							texture.id = texFromFile(filename.c_str(), directory);
							texture.type = typeName;
							texture.path = _file.path().string();
							textures.push_back(texture);
							texLoaded.push_back(texture);
						}
					}
					else
						if (type == aiTextureType_HEIGHT)
						{
							Texture texture;

							if ((filename.find("height") != string::npos) || (filename.find("heights") != string::npos))
							{
								texture.id = texFromFile(filename.c_str(), directory);
								texture.type = typeName;
								texture.path = _file.path().string();
								textures.push_back(texture);
								texLoaded.push_back(texture);
							}
						}
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
	//stbi_set_flip_vertically_on_load(true);
	if (imgData) {
		GLenum format = 0;
		if (imgCh == 1) format = GL_RED;
		if (imgCh == 3) format = GL_RGB;
		if (imgCh == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, imgW, imgH, 0, format, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Warning: MODEL_H. Texture not loaded. Path: \"" << path << "\"\n";
	stbi_image_free(imgData);

	return id;
}