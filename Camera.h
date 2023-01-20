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
	void Boost();
	void AdjustVelocity(float* axes);
	void AdjustOrientation(float* axes);

	// mouse behavior callbacks
	void BindCursor();
	void UnbindCursor();
private:
	int BoostCharged = 2;
	float friction = 0.1f;
	float worldTime = 0.0f;
	float prevBoostTime = 0.0f;
	
	bool breaking = false;

	bool flatNav = false;
	glm::vec3 surfaceNormal = glm::vec3(0.0f,0.0f,0.0f);

	const float collisionAcceleration = 2.5f;
	const float baseAcceleration = 0.4f;
	const float jumpAcceleration = 2.00f;
	const float boostAcceleration = 10.00f;
	const float boostCooldown = 1.0f;
	const float maxSpeed = 6.0f;
	const float collisionFriction = 2.0f;
	const float baseFriction = 0.1f;
	const float gravity = 0.125f;
	const float repulsionFac = 1.1f;

	// TODO Move these to input Handler
	const float mouseLookSensitivity = 100.0f;
	const float joystickLookSensitivity = 10.0f;
};