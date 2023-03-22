#pragma once

#include "Shader.h"
#include "Opponent.h"
#include "Updatable.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Player:
	public Updatable,
	public Deletable
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
	void Delete();

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

	bool lockPressed = false;
	bool locked = false;

	glm::vec3 spawnPoint;
	glm::vec3 lastCollision;
	const double fireInterval = 0.1;
	double prevFireTime = 0;
	const double jumpInterval = 0.1;
	double prevJumpTime = 0;

	Opponent& opponent;
	glm::vec3 surfaceNormal = glm::vec3(0.0f,0.0f,0.0f);

	bool DashForwardReady, DashBackReady, DashLeftReady, DashRightReady, JumpReady;
	void DirectionalDash(glm::vec3 direction, bool& ready);

	const float collisionAcceleration = 2000.0f;
	const float friction = 200.0f;
	const float minGroundVelocity = 10.0f;
	const float energyBound = 4000.0f;
	const float airAcceleration = 1000.0f;

	const float breakFactor = 10.0f;
	const float jumpVelocity = 350.00f;
	const float dashAcceleration = 5000.00f;
	const float dashInitVelocity = 175.0f;
	const float maxSpeed = 175.0f; // for acceleraton only
	const float gravity = 700.0f;
	const float repulsionFac = 1.2;

	const float lowEnergyThreshold = 0.1;
	const float energyInitDash = 0.09f;
	const float energySustainDash = 0.01f;
	const float energyRegenRate = 0.5f;
	const float firingEnergy = 0.03f;

	const float maxLookSensitivity = 300.0f;
	const float minLookSensitivity = 40.0f;
	const float lockAngle = acos(0) / 10;
};
