#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 startPosition):
	windowWidth(width), 
	windowHeight(height), 
	position(startPosition)
{}

void Camera::update(float FOVdeg, float nearPlane, float farPlane)
{
	// Makes camera look in the right direction from the right position
	view = glm::lookAt(position, position + orientation, up);
	// Adds perspective to the scene
	float aspectRatio = (float)windowWidth / windowHeight;
	projection = glm::perspective(glm::radians(FOVdeg), aspectRatio, nearPlane, farPlane);
}

void Camera::setPositionUniform(Shader& shader, const char* uniformName)
{
	// Exports camera view uniform
	const char* name = uniformName ? uniformName : "position";
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), position.x, position.y, position.z);
}

void Camera::setViewUniform(Shader& shader, const char* uniformName)
{
	// Exports camera view uniform
	const char* name = uniformName ? uniformName : "view";
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, name), 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::setProjectionUniform(Shader& shader, const char* uniformName)
{
	// Exports camera projection uniform
	const char* name = uniformName ? uniformName : "projection";
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, name), 1, GL_FALSE, glm::value_ptr(projection));
}

// TODO_SOON move input handler
void Camera::Inputs(GLFWwindow* window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += speed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += speed * -glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += speed * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += speed * glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		position += speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		position += speed * -up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.1f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.025f;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
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
		float rotX = sensitivity * (float)(mouseY - (windowHeight / 2)) / windowHeight;
		float rotY = sensitivity * (float)(mouseX - (windowWidth / 2)) / windowWidth;

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
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}