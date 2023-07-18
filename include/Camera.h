#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

const float kYaw = -90.f;
const float kPitch = 0.f;
const float kSpeed = 2.f;
const float kSensitivity = 0.075f;
const float kZoom = 45.f;

class Camera {
public:

	enum Direction
	{
		FRONT = 1,
		RIGHT,
		BACK,
		LEFT
	};

	Camera(glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = kYaw, float pitch = kPitch);

	///////////////////////////////
	// Setters

	void setPos(glm::vec3 pos);
	void setMovementSpeed(float);
	void setSensitivity(float);

	///////////////////////////////
	// Transformations and move

	// Move in the direction
	void move(Direction direction, float deltaTime);
	
	// Look around
	void processMouseMovement(float xOffset, float yOffset, bool pitchConstrain = true);

	void turnBack();

	///////////////////////////////
	// Getters

	glm::vec3 getPos() const;
	glm::mat4 getView() const;
	float getMovementSpeed() const;

private:

	void updateVectors();

	// Set by constructor or manually
	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 worldUp;
	float yaw;
	float pitch;

	// Set by updateVectors()
	glm::vec3 front;
	glm::vec3 right;

	// Transform and move parameters
	float movementSpeed;
	float mouseSensitivity;

	//glm::mat4 view;
};

class ZoomCamera : public Camera
{
public:

	ZoomCamera(glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
			   float yaw = kYaw, float pitch = kPitch, float zoom = kZoom);

	void processMouseScroll(float yOffset);

	// In development ...

private:

	float cameraZoom;

};

#endif
