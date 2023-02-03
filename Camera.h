#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	// Camera description Vectors
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// window dimensions
	int windowWidth;
	int windowHeight;
	
	GLFWwindow* window;

	Camera(GLFWwindow* window, int width, int height, glm::vec3 startPosition);

	// Exports the POV related unifroms
	void SetCameraUniforms(Shader& shader);
	void SetSkyboxUniforms(Shader& skyboxShader);

	void Update(float time);

	// input movement
	void Forward();
	void TranslateLeft();
	void Back();
	void TranslateRight();
	void Jump();
	void DashForward();
	void DashBack();
	void DashLeft();
	void DashRight();
	void Break();
	void AdjustVelocity(float* axes);
	void AdjustOrientation(float* axes);

	// mouse behavior callbacks
	void BindCursor();
	void UnbindCursor();
private:
	float friction = 0.1f;

	bool flatNav = false;
	glm::vec3 surfaceNormal = glm::vec3(0.0f,0.0f,0.0f);

	const float collisionAcceleration = 2.5f;
	const float airAcceleration = 0.8f;
	const float jumpAcceleration = 5.00f;
	const float boostAcceleration = 5.00f;
	const float maxSpeed = 5.0f;
	const float collisionFriction = 2.0f;
	const float baseFriction = 0.1f;
	const float gravity = 0.25f;
	const float repulsionFac = 1.5f;
	const glm::vec3 modelAdjustment = glm::vec3(0.0f, 5.0f, 0.0f);

	// TODO Move these to input Handler
	const float mouseLookSensitivity = 100.0f;
	const float joystickLookSensitivity = 15.0f;
};