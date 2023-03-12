#pragma once

#include "Shader.h"
#include "Opponent.h"
#include "Updatable.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Player:
	public Updatable
{
public:
	// Camera description Vectors
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// TODO: figureout why window is blank without this******
	GLFWwindow* window; //***********************************
	//*******************************************************

	Player(glm::vec3 startPosition, Opponent& opponent, ProjectileStream& projectileStream);


	void Update(double time);

	// input movement
	void ZoomAndLock(float triggerValue);

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

	void FireProjectile(float intensity);

	void TakeDamage(float damage);
	void AdjustVelocity(float xAxis, float yAxis);
	void AdjustOrientation(float xAxis, float yAxis);
	float energy = 1.0f;
	float health = 100.0f;
	float zoom = -1.0;
	float feildOfView = 45.0f;
	bool flatNav = false;
	// TODO auto-zoom
private:

	ProjectileStream& projectileStream;
	void Reset();
	glm::vec3 spawnPoint;
	float currentTime = 0;
	const float fireInterval = 0.1f;
	float prevFireTime = 0;

	Opponent& opponent;
	float friction;
	glm::vec3 surfaceNormal = glm::vec3(0.0f,0.0f,0.0f);

	bool DashForwardReady, DashBackReady, DashLeftReady, DashRightReady = true;
	void DirectionalDash(glm::vec3 direction, bool& ready);

	const float collisionAcceleration = 4.0f;
	const float collisionFriction = 2.0f;

	const float airAcceleration = 0.8f;
	const float airFriction = 0.1f;

	const float jumpAcceleration = 7.00f;
	const float dashAcceleration = 2.00f;
	const float dashInitVelocity = 7.0f;
	const float maxSpeed = 5.0;
	const float gravity = 0.25f;
	const float repulsionFac = 1.5f;

	const float energyInitDash = 0.09f;
	const float energySustainDash = 0.01f;
	const float energyRegen = 0.002f;
	const float firingEnergy = 0.003f;

	const float maxLookSensitivity = 15.0f;
	const float minLookSensitivity = 2.0f;
	const float lockedLookSensitivity = 5.0f;
	const float lockAngle = acos(0) / 10;
};
