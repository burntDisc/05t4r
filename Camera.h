#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	// Camera description Vectors
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
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

	void Update();

	// input movement
	void Forward();
	void TranslateLeft();
	void Back();
	void TranslateRight();
	void TranslateUp();
	void AdjustVelocity(const float* axes);
	void AdjustOrientation(const float* axes);

	// mouse behavior callbacks
	void BindCursor();
	void UnbindCursor();
private:
	bool flatNav = false;
	glm::vec3 surfaceNormal = glm::vec3(0.0f,0.0f,0.0f);

	const float acceleration = 1.0f;
	const float jumpAcceleration = 2.00f;
	const float maxSpeed = 3.0f;
	const float friction = 0.1f;
	const float gravity = 0.125f;
	const float repulsionFac = 2.0f;

	// TODO Move these to input Handler
	const float joystickThreshold = 0.4;
	const float mouseLookSensitivity = 100.0f;
	const float joystickLookSensitivity = 20.0f;
};