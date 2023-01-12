#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/projection.hpp>

#include "MotionHandler.h"

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
	float nearPlane = 0.01;
	float farPlane = 100000.0;
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
	const float axes[2] = { 0.0, -1.0 };
	AdjustVelocity(axes);
}
void Camera::TranslateLeft()
{
	const float axes[2] = { -1.0, 0.0 };
	AdjustVelocity(axes);
}
void Camera::TranslateUp()
{
	if (flatNav)
	{
		glm::vec3 newVelocity = jumpAcceleration * up + velocity;

		velocity = glm::length(newVelocity) > maxSpeed ?
			normalize(newVelocity) * length(velocity) :
			newVelocity;
	}

}
void Camera::TranslateRight()
{
	const float axes[2] = { 1.0, 0.0 };
	AdjustVelocity(axes);
}
void Camera::Back()
{
	const float axes[2] = { 0.0, 1.0 };
	AdjustVelocity(axes);
}

void Camera::Update()
{
	if (glm::length(velocity) > friction)
	{
		velocity = velocity - friction * glm::normalize(velocity);
	}
	else
	{
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	glm::vec3 newPosition = MotionHandler::ApplyTranslation(position, position + velocity, surfaceNormal);
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
	}
	else
	{
		flatNav = false;
		velocity.y -= gravity;
	}
	position = newPosition;
}

void Camera::AdjustVelocity(const float* axes)
{
	if (abs(axes[0]) > joystickThreshold || abs(axes[1]) > joystickThreshold)
	{
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
		glm::vec3 newVelocity = acceleration * stickDirection + velocity;

		velocity = glm::length(newVelocity) > maxSpeed ?
			normalize(newVelocity) * length(velocity) :
			newVelocity;
	}
}

void Camera::AdjustOrientation(const float* axes)
{
	float rotX = 0;
	float rotY = 0;
	if (abs(axes[0]) > joystickThreshold || abs(axes[1]) > joystickThreshold)
	{
		rotX = joystickLookSensitivity * axes[1];
		rotY = joystickLookSensitivity * axes[0];
	}

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
