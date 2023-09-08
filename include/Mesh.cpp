#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->textures = textures;

	setupMesh();
}

// Vertices and single texture
Mesh::Mesh(vector<Vertex> vertices, Texture texture)
{
	this->vertices = vertices;
	this->textures.push_back(texture);

	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	if (this->indices.size() > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}

	//vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	
	if (this->indices.size() > 0)
	{
		//normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		//textures
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	}
	else /* receive only texture coords as second vertex layer */
	{	
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	}
	////tangent
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	//
	////bitangent
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	//
	////ids
	//glEnableVertexAttribArray(5);
	//glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));

	////weights
	//glEnableVertexAttribArray(6);
	//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

	glBindVertexArray(0);
}


void Mesh::draw(Shader& shader)
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	unsigned int normalNr = 0;
	//unsigned int heightNr = 0;

	shader.use();
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		string name;
		TextureType type = textures[i].type;
		if (type == TEXTURE_DIFFUSE)
		{
			number = std::to_string(diffuseNr++);
			name = "tex_diffuse";
		}
		if (type == TEXTURE_SPECULAR)
		{
			number = std::to_string(specularNr++);
			name = "tex_specular";
		}
		if (type == TEXTURE_NORMALS)
		{
			number = std::to_string(normalNr++);
			name = "tex_normal";
		}
		/*if (name == "tex_height")
		{
			number = std::to_string(heightNr++);
			name = "tex_height";
		}*/
		string tmp = "fuMaterial." + name + number;
		shader.setInt(tmp, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);
	
	//Draw full mesh 
	if (indices.size() > 0)
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

	//or

	//Draw square
	if (vertices.size() == 6)
		glDrawArrays(GL_TRIANGLES, 0, 6);

	//or

	//Draw cube
	if (vertices.size() == 36)
		glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

vector<Vertex> vertexDataToVertexVector(const float* vertexData, int size, VertexType vertexType)
{
	vector<Vertex> vec;
	
	switch (vertexType)
	{
		case VERTEX: //3 pos coords, 3 normal coords, 2 tex coords 
		{	
			int i = 0;
			while (i < size)
			{
				Vertex vert;
				vert.position.x = vertexData[i++];
				vert.position.y = vertexData[i++];
				vert.position.z = vertexData[i++];
				vert.normal.x = vertexData[i++];
				vert.normal.y = vertexData[i++];
				vert.normal.z = vertexData[i++];
				vert.texCoords.x = vertexData[i++];
				vert.texCoords.y = vertexData[i++];
				vec.push_back(vert);
			}

			break;
		}
		case VERTEX_P_T: //3 pos coords, 2 tex coords
		{
			int i = 0;
			while (i < size)
			{
				Vertex vert;
				vert.position.x = vertexData[i++];
				vert.position.y = vertexData[i++];
				vert.position.z = vertexData[i++];
				vert.texCoords.x = vertexData[i++];
				vert.texCoords.y = vertexData[i++];
				vec.push_back(vert);
			}
			break;
		}
		default: break;
	}
	return vec;
}

void flipTextureCoords(float* vertices, size_t size, VertexType vertexType)
{
	int n = size / sizeof(float);
	if (vertexType == VERTEX)
	{
		for (int i = 0; i < n; i++)
		{
			if (i % 6 == 0) vertices[i] = 1.f - vertices[i];
			if (i % 7 == 0) vertices[i] = 1.f - vertices[i];
		}
	}
	else
		if (vertexType == VERTEX_P_T)
		{
			for (int i = 0; i < n / 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					if (j == 4) *vertices = 1.f - *vertices;
					vertices++;
				}
			}
		}
}

////////////////////////////////////////////////
// Texture loading
//////////////////////

void Mesh::loadMeshTexture(const char* path, GLenum wrapMode, TextureType type)
{
	Texture texture;
	texture.id = loadTex(path, wrapMode);
	texture.path = path;
	texture.type = type;
	textures.push_back(texture);
}

unsigned int loadTex(const char* path, GLenum wrapMode) {
	unsigned int id;
	glGenTextures(1, &id);
	int imgW, imgH, imgCh;
	unsigned char* imgData = stbi_load(path, &imgW, &imgH, &imgCh, 0);
	//stbi_set_flip_vertically_on_load(true);
	if (imgData) {
		GLenum format = 0;
		if (imgCh == 1) format = GL_RED;
		if (imgCh == 3) format = GL_RGB;
		if (imgCh == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, imgW, imgH, 0, format, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (wrapMode == GL_REPEAT)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else if (wrapMode == GL_CLAMP_TO_EDGE)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cerr << "Warning: Texture not loaded\n";
	stbi_image_free(imgData);

	return id;
};

Texture loadTexTex(const char* path, GLenum wrapMode, TextureType type)
{
	Texture texture;
	texture.id = loadTex(path, wrapMode);
	texture.path = path;
	texture.type = type;
	return texture;
}

unsigned int loadCubemap(const char* dirPath, std::vector<std::string> faces)
{
	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int w, h, ch;
	for (int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load((std::string(dirPath) + "\\\\" + faces[i]).c_str(), &w, &h, &ch, 0);
		if (!data)
		{
			std::cerr << "Warning: loadCubemap(). texture \"" << faces[i] << "\" not found\n";
			stbi_image_free(data);
			continue;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	return id;
};