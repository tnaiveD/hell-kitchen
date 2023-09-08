#include "Shader.h"

Shader::Shader() {}

Shader::Shader(const char* vertexSourcePath, const char* fragmentSourcePath, const char* geometrySourcePath) {

	//Read shader source from files
	//------------------------------
	std::string vertexSource;
	std::string fragmentSource;
	std::ifstream vertexIfStream;
	std::ifstream fragmentIfStream;

	//exceptions
	vertexIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vertexIfStream.open(vertexSourcePath);
		fragmentIfStream.open(fragmentSourcePath);

		std::stringstream vertexStream, fragmentStream;

		vertexStream << vertexIfStream.rdbuf();
		fragmentStream << fragmentIfStream.rdbuf();
		
		vertexIfStream.close();
		fragmentIfStream.close();

		vertexSource = vertexStream.str();
		fragmentSource = fragmentStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error: SHADER_CPP. File not successfuly read\n";
	}
	const char* vertexShaderSource = vertexSource.c_str();
	const char* fragmentShaderSource = fragmentSource.c_str();

	//geometry shader
	std::string geometrySource;

	if (geometrySourcePath)
	{
		std::ifstream geometryIfStream;
		geometryIfStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		
		try
		{
			geometryIfStream.open(geometrySourcePath);

			std::stringstream geometryStream;
			geometryStream << geometryIfStream.rdbuf();

			geometryIfStream.close();
			geometrySource = geometryStream.str();

		}
		catch (std::ifstream::failure e)
		{
			std::cerr << "Geometry Source File not successfuly read\n";
		}
	}


	//Compiling shaders
	//------------------------------
	int success;
	char infoLog[512];

	//vertex
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error: SHADER_CPP (" << vertexSourcePath << ", " 
				  << fragmentSourcePath << "). Vertex Shader compilation FAILED\n" << infoLog << '\n';
	}

	//fragment
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error: SHADER_CPP (" << vertexSourcePath << ", "
			<< fragmentSourcePath << "). Fragment Shader compilation FAILED\n" << infoLog << '\n';
	}

	//geometry
	unsigned int shaderGeometry;
	if (geometrySourcePath) 
	{
		const char* geometrySourceCStr = geometrySource.c_str();
		shaderGeometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(shaderGeometry, 1, &geometrySourceCStr, NULL);
		glCompileShader(shaderGeometry);
		glGetShaderiv(shaderGeometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderGeometry, sizeof(infoLog), NULL, infoLog);
			std::cerr << "Error: Geometry Shader. " << infoLog << "\n";
		}
	}

	//shader program
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	if (geometrySourcePath) 
		glAttachShader(id, shaderGeometry);
	
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error: SHADER_CPP (" << vertexSourcePath << ", "
			<< fragmentSourcePath << "). Shader Program linking FAILED\n" << infoLog << '\n';
	}

	//clear
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometrySourcePath)
		glDeleteShader(shaderGeometry);

	//uniforms check
	if (geometrySourcePath)
	{
		this->infoLog += std::string(vertexSourcePath) + std::string(", ") +
			std::string(fragmentSourcePath) + std::string(", ") +
			std::string(geometrySourcePath) + std::string(": \n");
	}
	else
	{
		this->infoLog += std::string(vertexSourcePath) + std::string(", ") +
			std::string(fragmentSourcePath) + std::string(": \n");
	}

	wrongUniforms.clear();
}

void Shader::use() const {
	glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool val) const{
	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		if (wrongUniforms.find(name) == wrongUniforms.end())
		{
			infoLog += std::string("- Uniform \"" + name + "\" does not exist or use.\n");
			wrongUniforms.insert(name);
		}
	}
	else
		glUniform1i(location, val);
}

void Shader::setInt(const std::string& name, int val) const{
	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		if (wrongUniforms.find(name) == wrongUniforms.end())
		{
			infoLog += std::string("- Uniform \"" + name + "\" does not exist or use.\n");
			wrongUniforms.insert(name);
		}
	}
	else
		glUniform1i(location, val);
}

void Shader::setFloat(const std::string& name, float val) const{
	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		if (wrongUniforms.find(name) == wrongUniforms.end())
		{
			infoLog += std::string("- Uniform \"" + name + "\" does not exist or use.\n");
			wrongUniforms.insert(name);
		}
	}
	else
		glUniform1f(location, val);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const{
	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		if (wrongUniforms.find(name) == wrongUniforms.end())
		{
			infoLog += std::string("- Uniform \"" + name + "\" does not exist or use.\n");
			wrongUniforms.insert(name);
		}
	}
	else
		glUniform3f(location, x, y, z);
}

void Shader::setVec3(const std::string& name, glm::vec3 vec) const {
	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		if (wrongUniforms.find(name) == wrongUniforms.end())
		{
			infoLog += std::string("- Uniform \"" + name + "\" does not exist or use.\n");
			wrongUniforms.insert(name);
		}
	}
	else
		glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::setVec2(const std::string& name, float x, float y) const {
	int location = glGetUniformLocation(id, name.c_str());
	
	if (location == -1)
	{
		if (wrongUniforms.find(name) == wrongUniforms.end())
		{
			infoLog += std::string("- Uniform \"" + name + "\" does not exist or use.\n");
			wrongUniforms.insert(name);
		}
	}
	else
		glUniform2f(location, x, y);
}

void Shader::setVec2(const std::string& name, glm::vec2 vec) const {
	int location = glGetUniformLocation(id, name.c_str());

	if (location == -1)
	{
		if (wrongUniforms.find(name) == wrongUniforms.end())
		{
			infoLog += std::string("- Uniform \"" + name + "\" does not exist or use.\n");
			wrongUniforms.insert(name);
		}
	}
	else
		glUniform2f(location, vec.x, vec.y);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat4) const {
	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		if (wrongUniforms.find(name) == wrongUniforms.end())
		{
			infoLog += std::string("- Uniform \"" + name + "\" does not exist or use.\n");
			wrongUniforms.insert(name);
		}
	}
	else
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}

unsigned int Shader::getID() const { return id; }

void Shader::setDLight(const DirectionalLight& light) const
{
	this->setVec3("DLight.ambient", light.getAmbient());
	this->setVec3("DLight.diffuse", light.getDiffuse());
	this->setVec3("DLight.specular", light.getSpecular());
	
	this->setVec3("DLight.dir",	light.getDir());

	this->setBool("DLight.active", light.isActive());
};


void Shader::setPLights(const std::vector<PointLight*>& light) const
{
	this->setInt("PLnum", PointLight::getCount());

	for (int i = 0; i < light.size(); i++)
	{
		this->setVec3("PLight[" + std::to_string(i) + "].ambient", light[i]->getAmbient());
		this->setVec3("PLight[" + std::to_string(i) + "].diffuse", light[i]->getDiffuse());
		this->setVec3("PLight[" + std::to_string(i) + "].specular", light[i]->getSpecular());
		
		this->setVec3("PLight[" + std::to_string(i) + "].pos", light[i]->getPos());
		
		glm::vec3 atten = light[i]->getAttenuation();
		this->setFloat("PLight[" + std::to_string(i) + "].constant", atten.x);
		this->setFloat("PLight[" + std::to_string(i) + "].linear", atten.y);
		this->setFloat("PLight[" + std::to_string(i) + "].quadratic", atten.z);
	
		this->setBool("PLight[" + std::to_string(i) + "].active", light[i]->isActive());
	}
};

void Shader::setPLights(const PointLight& light, int i) const
{
	this->setInt("PLnum", PointLight::getCount());
	
	this->setVec3("PLight[" + std::to_string(i) + "].ambient", light.getAmbient());
	this->setVec3("PLight[" + std::to_string(i) + "].diffuse", light.getDiffuse());
	this->setVec3("PLight[" + std::to_string(i) + "].specular", light.getSpecular());
	
	this->setVec3("PLight[" + std::to_string(i) + "].pos", light.getPos());
	
	glm::vec3 atten = light.getAttenuation();
	this->setFloat("PLight[" + std::to_string(i) + "].constant", atten.x);
	this->setFloat("PLight[" + std::to_string(i) + "].linear", atten.y);
	this->setFloat("PLight[" + std::to_string(i) + "].quadratic", atten.z);	

	this->setBool("PLight[" + std::to_string(i) + "].active", light.isActive());
};

void Shader::setSLights(const std::vector<SpotLight*>& light) const
{
	this->setInt("SLnum", SpotLight::getCount());
	for (int i = 0; i < light.size(); i++)
	{
		this->setVec3("SLight[" + std::to_string(i) + "].ambient", light[i]->getAmbient());
		this->setVec3("SLight[" + std::to_string(i) + "].diffuse", light[i]->getDiffuse());
		this->setVec3("SLight[" + std::to_string(i) + "].specular", light[i]->getSpecular());
		
		this->setVec3("SLight[" + std::to_string(i) + "].pos", light[i]->getPos());
		this->setVec3("SLight[" + std::to_string(i) + "].dir", light[i]->getDir());
		
		glm::vec3 atten = light[i]->getAttenuation();
		this->setFloat("SLight[" + std::to_string(i) + "].constant", atten.x);
		this->setFloat("SLight[" + std::to_string(i) + "].linear", atten.y);
		this->setFloat("SLight[" + std::to_string(i) + "].quadratic", atten.z);
		
		this->setFloat("SLight[" + std::to_string(i) + "].inAngle", glm::cos(glm::radians(light[i]->getInAngle())));
		this->setFloat("SLight[" + std::to_string(i) + "].outAngle", glm::cos(glm::radians(light[i]->getOutAngle())));

		this->setBool("SLight[" + std::to_string(i) + "].active", light[i]->isActive());
	}
};

void Shader::setSLights(const SpotLight& light, int i) const
{		
	this->setInt("SLnum", SpotLight::getCount());

	this->setVec3("SLight[" + std::to_string(i) + "].ambient", light.getAmbient());
	this->setVec3("SLight[" + std::to_string(i) + "].diffuse", light.getDiffuse());
	this->setVec3("SLight[" + std::to_string(i) + "].specular", light.getSpecular());
	
	this->setVec3("SLight[" + std::to_string(i) + "].pos", light.getPos());
	this->setVec3("SLight[" + std::to_string(i) + "].dir", light.getDir());
	
	glm::vec3 atten = light.getAttenuation();
	this->setFloat("SLight[" + std::to_string(i) + "].constant", atten.x);
	this->setFloat("SLight[" + std::to_string(i) + "].linear", atten.y);
	this->setFloat("SLight[" + std::to_string(i) + "].quadratic", atten.z);
	
	this->setFloat("SLight[" + std::to_string(i) + "].inAngle", glm::cos(glm::radians(light.getInAngle())));
	this->setFloat("SLight[" + std::to_string(i) + "].outAngle", glm::cos(glm::radians(light.getOutAngle())));
	
	this->setBool("SLight[" + std::to_string(i) + "].active", light.isActive());
};

void Shader::suicide() {
	glDeleteProgram(id);
}


////////////////////////////////
// Debug

std::string Shader::getInfoLog() const
{
	if (wrongUniforms.empty())
	{
		return std::string(infoLog + "OK!\n");
	}
	else
	{
		return infoLog;
	}
}

