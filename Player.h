#pragma once

#include "Shader.h"
#include "Opponent.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Player
{
public:
	// Camera description Vectors
	glm::vec3 translation;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// window dimensions
	int windowWidth;
	int windowHeight;
	
	GLFWwindow* window;

	Player(GLFWwindow* window, int width, int height, glm::vec3 startPosition, Opponent* opponent);

	// Exports the POV related unifroms
	void SetCameraUniforms(Shader& shader);
	void SetSkyboxUniforms(Shader& skyboxShader);

	void Update(float time);

	// input movement
	void ZoomAndLock(float* triggerValue);
	void Forward();
	void TranslateLeft();
	void Back();
	void TranslateRight();
	void Jump();
	void DashForward();
	void DashBack();
	void DashLeft();
	void DashRight();
	void ReadyDashForward();
	void ReadyDashBack();
	void ReadyDashLeft();
	void ReadyDashRight();
	void Break();
	void AdjustVelocity(float* axes);
	void AdjustOrientation(float* axes);

	float energy = 1.0f;
private:

	Opponent* opponent;
	bool targetLocked = false;
	float friction;
	bool flatNav = false;
	float feildOfView = 45.0f;
	glm::vec3 surfaceNormal = glm::vec3(0.0f,0.0f,0.0f);

	bool DashForwardReady, DashBackReady, DashLeftReady, DashRightReady = true;
	void DirectionalDash(glm::vec3 direction, bool& ready);

	const float collisionAcceleration = 4.0f;
	const float collisionFriction = 2.0f;

	const float airAcceleration = 0.8f;
	const float airFriction = 0.1f;

	const float jumpAcceleration = 7.00f;
	const float dashAcceleration = 2.00f;
	const float dashInitVelocity = 10.0f;
	const float maxSpeed = 5.0;
	const float gravity = 0.25f;
	const float repulsionFac = 1.5f;

	const float energyDash = 0.01f;
	const float energyRegen = 0.002f;

	const float defaultLookSensitivity = 15.0f;
	const float lockedLookSensitivity = 5.0f;
};