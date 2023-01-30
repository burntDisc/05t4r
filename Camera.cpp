#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/projection.hpp>

#include "MotionHandler.h"
#include "NetworkHandler.h"

#include <iostream> // remove

Camera::Camera(GLFWwindow* window, int width, int height, glm::vec3 startPosition) :
	window(window),
	windowWidth(width),
	windowHeight(height),
	position(startPosition)
{}

void Camera::SetCameraUniforms(Shader& shader)
{
	float feildOfView = 45.0f;
	float nearPlane = 0.01f;
	float farPlane = 100000.0f;
	// Makes camera look in the right direction from the right position
	view = glm::lookAt(position, position + orientation, up);
	// Adds perspective to the scene
	float aspectRatio = (float)windowWidth / windowHeight;

	projection = glm::perspective(glm::radians(feildOfView), aspectRatio, nearPlane, farPlane);

	shader.Activate();
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), position.x, position.y, position.z);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::SetSkyboxUniforms(Shader& skyboxShader)
{

	skyboxShader.Activate();
	glm::mat4 skyboxView = glm::mat4(1.0f);
	glm::mat4 skyboxProjection = glm::mat4(1.0f);

	// mat4 -> mat3 -> mat4 removes last row and column for translation
	skyboxView = glm::mat4(glm::mat3(glm::lookAt(position, position + orientation, up)));
	skyboxProjection = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(skyboxProjection));
}

void Camera::Forward()
{
	float axes[2] = { 0.0, -1.0 };
	AdjustVelocity(axes);
}

void Camera::TranslateLeft()
{
	float axes[2] = { -1.0, 0.0 };
	AdjustVelocity(axes);
}

void Camera::Jump()
{
	if (flatNav)
	{
		velocity = jumpAcceleration * up + velocity;
	}

}
void Camera::Boost()
{
	if(BoostCharged && worldTime - prevBoostTime > boostCooldown)
	{
		glm::vec3 adjustment = breaking ?
			- velocity :
			boostAcceleration * orientation;
		velocity = adjustment + velocity;
		--BoostCharged;
	}
}
void Camera::TranslateRight()
{
	float axes[2] = { 1.0, 0.0 };
	AdjustVelocity(axes);
}
void Camera::Back()
{
	float axes[2] = { 0.0, 1.0 };
	AdjustVelocity(axes);
}

void Camera::Update(float time)
{
	worldTime = time;
	if (glm::length(velocity) > friction)
	{
		velocity = velocity - friction * glm::normalize(velocity);
	}
	else
	{
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	glm::vec3 newPosition = MotionHandler::CollideAndSlide(position, velocity, surfaceNormal);
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
		BoostCharged = 3;
	}
	else
	{
		flatNav = false;
		velocity.y -= gravity;
		friction = baseFriction;
	}
	position = newPosition;
	auto state = NetworkHandler::GetGamestate();
	state.position = newPosition;
	state.orientation = orientation;
	state.position += modelAdjustment;
	NetworkHandler::SetGamestate(state);
}

void Camera::AdjustVelocity(float* axes)
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

	breaking = glm::dot(stickDirection, orientation) < 0;

	// apply change from input
	glm::vec3 newVelocity =
		flatNav ?
		collisionAcceleration * stickDirection + velocity :
		baseAcceleration * stickDirection + velocity;

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

void Camera::AdjustOrientation(float* axes)
{
	float rotX = joystickLookSensitivity * axes[1];
	float rotY = joystickLookSensitivity * axes[0];

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

void Camera::BindCursor()
{
	//TODO finish moving mouse logic to inputhandler
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Prevents camera from jumping on the first click
	if (firstClick)
	{
		glfwSetCursorPos(window, (windowWidth / 2), (windowHeight / 2));
		firstClick = false;
	}

	// Stores the coordinates of the cursor
	double mouseX;
	double mouseY;
	// Fetches the coordinates of the cursor
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
	// and then "transforms" them into degrees 
	float rotX = mouseLookSensitivity * (float)(mouseY - (windowHeight / 2)) / windowHeight;
	float rotY = mouseLookSensitivity * (float)(mouseX - (windowWidth / 2)) / windowWidth;

	// Calculates upcoming vertical change in the orientation
	glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

	// Decides whether or not the next vertical orientation is legal or not
	if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		orientation = newOrientation;
	}

	// Rotates the orientation left and right
	orientation = glm::rotate(orientation, glm::radians(-rotY), up);

	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	glfwSetCursorPos(window, (windowWidth / 2), (windowHeight / 2));


}

void Camera::UnbindCursor()
{		
	// Unhides cursor since camera is not looking around anymore
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// Makes sure the next time the camera looks around it doesn't jump
	firstClick = true;
}
