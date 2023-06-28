#ifndef CAMFPS_H
#define CAMFPS_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class CamFPS {
public:
	CamFPS();
	CamFPS(glm::vec3, glm::vec3, glm::vec3);

	void setView(glm::vec3, glm::vec3, glm::vec3);
	void setPos(glm::vec3);
	void setPosX(float);
	void setPosY(float);
	void setPosZ(float);
	void setDir(glm::vec3);
	void setUp(glm::vec3);
	void setSpeed(float);
	void setDeltaTime(float);

	void moveFront();
	void moveBack();
	void moveRight();
	void moveLeft();
	void moveDir(glm::vec3);

	glm::mat4& getView();
	float getSpeed() const;
	glm::vec3 getPos() const;
	glm::vec3 getDir() const;

private:
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 up;

	float speed;
	float deltaTime;

	//Pass this mat4 in fragment shader via getView()
	glm::mat4 view;
};



#endif
