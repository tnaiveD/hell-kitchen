#ifndef SHADER_H
#define SHADER_H

#include <gl.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader {
public:
	unsigned int id;

	Shader();
	Shader(const char* vertexSourcePath, const char* fragmentSourcePath);
	
	void use();
	
	//utility uniform setters
	void setBool(const std::string&, bool) const;
	void setInt(const std::string&, int) const;
	void setFloat(const std::string&, float) const;
	void setVec3(const std::string&, float, float, float) const;
	void setVec3(const std::string&, glm::vec3) const;
	void setMat4(const std::string&, glm::mat4&) const;

	void suicide();
};

#endif