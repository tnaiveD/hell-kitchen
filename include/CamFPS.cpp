#include "CamFPS.h"

CamFPS::CamFPS() {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	dir = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 0.0f, 0.0f);
	view = glm::mat4(1.0f);
	speed = 4.0f;
	deltaTime = 0.0f;
}

CamFPS::CamFPS(glm::vec3 pos, glm::vec3 dir, glm::vec3 up) : CamFPS(){
	this->pos = pos;
	this->dir = glm::normalize(dir);
	this->up = up;
	view = glm::lookAt(
		this->pos,
		this->pos + this->dir,
		this->up
	);
}

void CamFPS::setView(glm::vec3 pos, glm::vec3 dir, glm::vec3 up) {
	this->pos = pos;
	this->dir = dir;
	this->up = up;
	view = glm::lookAt(
		this->pos,
		this->pos + this->dir,
		this->up
	);
}

void CamFPS::setPos(glm::vec3 pos) {
	this->pos = pos;
	view = glm::lookAt(
		this->pos,
		this->pos + dir,
		up
	);
}

void CamFPS::setPosX(float val) {
	pos.x = val;
	view = glm::lookAt(
		pos,
		pos + dir,
		up
	);
}

void CamFPS::setPosY(float val) {
	pos.y = val;
	view = glm::lookAt(
		pos,
		pos + dir,
		up
	);
}

void CamFPS::setPosZ(float val) {
	pos.z = val;
	view = glm::lookAt(
		pos,
		pos + dir,
		up
	);
}

void CamFPS::setDir(glm::vec3 dir) {
	this->dir = dir;
	view = glm::lookAt(
		pos,
		pos + this->dir,
		up
	);
}

void CamFPS::setUp(glm::vec3 up) {
	this->up = up;
	view = glm::lookAt(
		pos,
		pos + dir,
		this->up
	);
}

void CamFPS::setSpeed(float speed) {
	this->speed = speed;
}

void CamFPS::setDeltaTime(float deltaTime) {
	this->deltaTime = deltaTime;
}

void CamFPS::moveFront() {
	setPos(pos + dir * speed * deltaTime);
}

void CamFPS::moveBack() {
	setPos(pos - dir * speed * deltaTime);
}

void CamFPS::moveLeft() {
	setPos(pos - glm::normalize(glm::cross(dir, up)) * speed * deltaTime);
}

void CamFPS::moveRight() {
	setPos(pos + glm::normalize(glm::cross(dir, up)) * speed * deltaTime);
}

glm::mat4& CamFPS::getView() {
	return view;
}

float CamFPS::getSpeed() const {
	return speed;
}

glm::vec3 CamFPS::getPos() const{
	return pos;
}

glm::vec3 CamFPS::getDir() const {
	return dir;
}
