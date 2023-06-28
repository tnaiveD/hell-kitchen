#include "Shader.h"

Shader::Shader(const char* vertexSourcePath, const char* fragmentSourcePath) {

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

	//shader program
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
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

	//uniforms check
	this->infoLog += std::string(vertexSourcePath) + std::string(", ") +
					 std::string(fragmentSourcePath) + std::string(": \n");

	wrongUniforms.clear();
}

void Shader::use() {
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

void Shader::setMat4(const std::string& name, glm::mat4& mat4) const {
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

