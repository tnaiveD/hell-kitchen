#ifndef SHADER_H
#define SHADER_H

#include <gl.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "Light.h"

class Shader {
public:
	unsigned int id;

	Shader() = delete;
	explicit Shader(const char* vertexSourcePath, const char* fragmentSourcePath, const char* geometrySourcePath = NULL);
	
	void use();
	
	//set uniforms
	void setBool(const std::string& name, bool) const;
	void setInt(const std::string& name, int) const;
	void setFloat(const std::string& name, float) const;
	void setVec3(const std::string& name, float, float, float) const;
	void setVec3(const std::string& name, glm::vec3) const;
	void setVec2(const std::string& name, float, float) const;
	void setVec2(const std::string& name, glm::vec2) const;
	void setMat4(const std::string& name, glm::mat4&) const;

	//get
	unsigned int getID() const;

	//debug
	std::string getInfoLog() const;

	//light data transfer
	void setDLight(DirectionalLight&) const;
	void setPLights(std::vector<PointLight*>&) const;
	void setSLights(std::vector<SpotLight*>&) const;

	//dealloc
	void suicide();
private:
	
	//debug
	mutable std::string infoLog;
	mutable std::set<std::string> wrongUniforms;
};

#endif