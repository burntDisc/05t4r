#include "Player.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/projection.hpp>

#include "MotionHandler.h"
#include "NetworkHandler.h"

#include <iostream> // remove

Player::Player(GLFWwindow* window, int width, int height, glm::vec3 startPosition, Opponent& opponent) :
	opponent(opponent),
	window(window),
	windowWidth(width),
	windowHeight(height),
	translation(startPosition)
{}

void Player::SetCameraUniforms(Shader& shader)
{
	float nearPlane = 0.01f;
	float farPlane = 7000.0f;
	// Makes camera look in the right direction from the right position
	view = glm::lookAt(translation, translation + orientation, up);
	// Adds perspective to the scene
	float aspectRatio = (float)windowWidth / windowHeight;

	projection = glm::perspective(glm::radians(feildOfView), aspectRatio, nearPlane, farPlane);

	shader.Activate();
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), translation.x, translation.y, translation.z);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Player::SetSkyboxUniforms(Shader& skyboxShader)
{

	skyboxShader.Activate();
	glm::mat4 skyboxView = glm::mat4(1.0f);
	glm::mat4 skyboxProjection = glm::mat4(1.0f);

	// mat4 -> mat3 -> mat4 removes last row and column for translation
	skyboxView = glm::mat4(glm::mat3(glm::lookAt(translation, translation + orientation, up)));
	skyboxProjection = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(skyboxProjection));
}

void Player::FireProjectile(float* intensity, ProjectileStream& projectileStream)
{
	if (currentTime - prevFireTime > fireInterval && *intensity > -0.99f)
	{
		energy -= firingEnergy;
		bool firing = true;
		NetworkHandler::SetLocalGamestate(NetworkHandler::firing, &firing);
		NetworkHandler::SetLocalGamestate(NetworkHandler::firingIntensity, intensity);
		projectileStream.Fire(translation, orientation, intensity);
	}
	else
	{
		bool notFiring = false;
		NetworkHandler::SetLocalGamestate(NetworkHandler::firing, &notFiring);
	}
}

void Player::Forward()
{
	float axes[2] = { 0.0, -1.0 };
	AdjustVelocity(axes);
}

void Player::TranslateLeft()
{
	float axes[2] = { -1.0, 0.0 };
	AdjustVelocity(axes);
}

void Player::Jump()
{
	if (flatNav)
	{
		velocity = jumpAcceleration * up + velocity;
	}

}

void Player::DirectionalDash(glm::vec3 direction, bool& ready)
{
	if (ready)
	{
		if (energy >= energyInitDash)
		{
			energy -= energyInitDash;
		}
		if (energy > energyInitDash)
		{
			ready = false;
			velocity = glm::dot(direction, velocity) > 0.0f ?
				glm::proj(velocity, direction) + dashInitVelocity * direction :
				dashInitVelocity * direction;
		}
	}
	else
	{
		if (energy >= energySustainDash)
		{
			energy -= energySustainDash;
		}

		if (energy > energySustainDash)
		{
			velocity += dashAcceleration * direction;
		}
	}
}

void Player::ReadyDashForward()
{
	DashForwardReady = true;
}
void Player::DashForward()
{
	glm::vec3 direction = orientation;
	DirectionalDash(direction, DashForwardReady);
}
void Player::ReadyDashBack()
{
	DashBackReady = true;
}
void Player::DashBack()
{
	glm::vec3 direction = -orientation;
	DirectionalDash(direction, DashBackReady);
}
void Player::ReadyDashLeft()
{
	DashLeftReady = true;
}
void Player::DashLeft()
{
	glm::vec3 direction = - glm::normalize(glm::cross(orientation, up));
	DirectionalDash(direction, DashLeftReady);
}
void Player::ReadyDashRight()
{
	DashRightReady = true;
}
void Player::DashRight()
{
	glm::vec3 direction = glm::normalize(glm::cross(orientation, up));
	DirectionalDash(direction, DashRightReady);
}

void Player::Break()
{
	velocity -= velocity / 2.0f;
}

void Player::TranslateRight()
{
	float axes[2] = { 1.0, 0.0 };
	AdjustVelocity(axes);
}
void Player::ZoomAndLock(float* triggerValue)
{
	zoom = *triggerValue;
	feildOfView = ((2.0f - *triggerValue) / 2.0f) * 35.0f + 10.0f;
	if (*triggerValue == 1.0f)
	{
		glm::vec3 target = opponent.GetPosition();
		glm::vec3 targetDirection = normalize(target - translation);
		if (glm::angle(targetDirection, orientation) < acos(0)/10) // 9deg
		{
			targetLocked = true;
			orientation = targetDirection;
		}
	}
	else
	{
		targetLocked = false;
	}
}

void Player::Back()
{
	float axes[2] = { 0.0, 1.0 };
	AdjustVelocity(axes);
}

void Player::Update(float time)
{
	currentTime = time;
	if (energy <= 1.0f - energyRegen) {
		energy += energyRegen;
	}

	glm::vec3 newTranslation = translation;
	if (glm::length(velocity) > friction)
	{
		velocity = velocity - friction * glm::normalize(velocity);
		newTranslation = MotionHandler::CollideAndSlide(translation, velocity, surfaceNormal);
	}
	else
	{
		if (surfaceNormal == glm::vec3(0.0, 0.0, 0.0))
		{
			newTranslation = MotionHandler::CollideAndSlide(translation, velocity, surfaceNormal);
		}
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	if (surfaceNormal != glm::vec3(0.0, 0.0, 0.0))
	{
		flatNav = true;
		float dot = glm::dot(velocity, surfaceNormal);
		// confirm velocity is towards surface
		if (dot < 0.0f)
		{	
			// remove towards-surface component
			glm::vec3 proj = glm::proj(velocity, surfaceNormal);
			velocity = velocity - repulsionFac*proj;
		}
		friction = collisionFriction;
	}
	else
	{
		velocity.y -= gravity;
		flatNav = false;
		friction = airFriction;
	}
	translation = newTranslation;
	if (targetLocked)
	{
		glm::vec3 target = opponent.GetPosition();
		glm::vec3 orientation = normalize(target - translation);
	}

	NetworkHandler::SetLocalGamestate(NetworkHandler::position, &newTranslation);
	NetworkHandler::SetLocalGamestate(NetworkHandler::orientation, &orientation);
}

void Player::AdjustVelocity(float* axes)
{
	// Allows for flat movement as opposed to flying
	bool alt = true;

	glm::vec3 relativeUp = surfaceNormal == glm::vec3(0.0, 0.0, 0.0) ? up : surfaceNormal;
	glm::vec3 normalizedSide = alt ? 
		glm::normalize(glm::cross(orientation, relativeUp)) : 
		glm::normalize(glm::cross(orientation, up));
	glm::vec3 normalizedFront = glm::normalize(glm::cross(relativeUp, normalizedSide));
	glm::vec3 stickSideComponent = axes[0] * normalizedSide;
	glm::vec3 stickFrontComponent = alt ? 
		- axes[1] * normalizedFront: 
		- axes[1] * orientation;
	glm::vec3 stickDirection = normalize(stickSideComponent + stickFrontComponent);

	// apply change from input
	glm::vec3 newVelocity =
		flatNav ?
		collisionAcceleration * stickDirection + velocity :
		airAcceleration * stickDirection + velocity;

	//removing y component to ignore gravity
	glm::vec3 newHorizontalVelocity = glm::vec3(newVelocity.x, 0.0, newVelocity.z);
	glm::vec3 horizontalVelocity = glm::vec3(velocity.x, 0.0, velocity.z);

	// change direction but do not increase speed if at max (excluding vertical)
	glm::vec3 calibratedHorizontalVelocity = glm::length(newHorizontalVelocity) > maxSpeed ?
		normalize(newHorizontalVelocity) * length(horizontalVelocity) :
		newHorizontalVelocity;

	velocity = glm::vec3(
		calibratedHorizontalVelocity.x,
		newVelocity.y,
		calibratedHorizontalVelocity.z);
}

void Player::AdjustOrientation(float* axes)
{
	float rotX = targetLocked? 
		lockedLookSensitivity * axes[1] : 
		defaultLookSensitivity * axes[1];
	float rotY = targetLocked ? 
		lockedLookSensitivity * axes[0] : 
		defaultLookSensitivity * axes[0];

	// Calculates upcoming vertical change in the orientation
	glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

	// Decides whether or not the next vertical orientation is legal or not
	if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		orientation = newOrientation;
	}

	// Rotates the orientation left and right
	orientation = glm::rotate(orientation, glm::radians(-rotY), up);
}
