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

	Shader();
	explicit Shader(const char* vertexSourcePath, const char* fragmentSourcePath, const char* GeometrySourcePath = NULL);
	
	void use() const;
	
	//set uniforms
	void setBool(const std::string& name, bool) const;
	void setInt(const std::string& name, int) const;
	void setFloat(const std::string& name, float) const;
	void setVec3(const std::string& name, float, float, float) const;
	void setVec3(const std::string& name, glm::vec3) const;
	void setVec2(const std::string& name, float, float) const;
	void setVec2(const std::string& name, glm::vec2) const;
	void setMat4(const std::string& name, const glm::mat4&) const;

	//get
	unsigned int getID() const;

	//debug
	std::string getInfoLog() const;

	////////////////////////////////////////////////
	// Light data transfer
	
	void setDLight(const DirectionalLight&) const;
	
	void setPLights(const std::vector<PointLight*>&) const;
	//simple light object, index
	void setPLights(const PointLight&, int i = 0) const;

	
	void setSLights(const std::vector<SpotLight*>&) const;
	//simple light object, index
	void setSLights(const SpotLight&, int i = 0) const;

	//dealloc
	void suicide();
private:
	
	//debug
	mutable std::string infoLog;
	mutable std::set<std::string> wrongUniforms;
};

#endif