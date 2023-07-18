#include "Camera.h"

/*
*	########################################
*	# PARENT CAMERA CLASS	  
*	########################################
*/

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch) 
	: movementSpeed(kSpeed), mouseSensitivity(kSensitivity)
{
	this->pos = pos;
	this->up = up;
	worldUp = this->up;
	this->yaw = yaw;
	this->pitch = pitch;

	updateVectors();
}

////////////////////////////////////////////
// Setters
//////////////////////

void Camera::updateVectors()
{
	glm::vec3 _front;
	_front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	_front.y = glm::sin(glm::radians(pitch));
	_front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front = glm::normalize(_front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::setPos(glm::vec3 pos) 
{
	this->pos = pos;
	
	updateVectors();
}

void Camera::setMovementSpeed(float speed) 
{
	movementSpeed = speed;
}

void Camera::setSensitivity(float sensitivity)
{
	mouseSensitivity = sensitivity;
}

////////////////////////////////////////////
// Transformation and move
///////////////////////

void Camera::move(Direction dir, float deltaTime)
{
	switch (dir)
	{
	case FRONT: { pos += front * (movementSpeed * deltaTime); break; }
	case RIGHT: { pos += right * (movementSpeed * deltaTime); break; }
	case BACK: { pos -= front * (movementSpeed * deltaTime); break; }
	case LEFT: { pos -= right * (movementSpeed * deltaTime); break; }
	}
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool pitchConstrain)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitchConstrain)
	{
		if (pitch > 89.f) pitch = 89.f;
		if (pitch < -89.f) pitch = -89.f;
	}

	updateVectors();
}

void Camera::turnBack()
{
	yaw += 180.f;
	updateVectors();
}

////////////////////////////////////////////
// Getters
/////////////////////

glm::mat4 Camera::getView() const {
	return glm::lookAt(pos, pos + front, up);
}

float Camera::getMovementSpeed() const {
	return movementSpeed;
}

glm::vec3 Camera::getPos() const{
	return pos;
}

//////////////////////////////////////////////

/*
*	########################################
*	# CAMERA -> ZOOMCAMERA
*	########################################
*/

ZoomCamera::ZoomCamera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch, float zoom)
	: Camera(pos, up, yaw, pitch)
{
	cameraZoom = zoom;
}

void ZoomCamera::processMouseScroll(float yOffset)
{
	cameraZoom -= yOffset;
	if (cameraZoom < 1.f)
		cameraZoom = 1.f;
	if (cameraZoom > 45.f)
		cameraZoom = 45.f;
}