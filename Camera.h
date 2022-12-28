#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"Shader.h"

class Camera
{
public:
	// Camera description Vectors
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// window dimensions
	int windowWidth;
	int windowHeight;

	// camera movement
	float speed = 10.0f;
	float sensitivity = 100.0f;

	Camera(int width, int height, glm::vec3 position);

	// Updates the camera matrix to the Vertex Shader
	void update(float FOVdeg, float nearPlane, float farPlane);

	// Exports the camera view uniform
	void setPositionUniform(Shader& shader, const char* uniformName = nullptr);

	// Exports the camera view uniform
	void setViewUniform(Shader& shader, const char* uniformName = nullptr);

	// Exports the camera projection uniform
	void setProjectionUniform(Shader& shader, const char* uniformName = nullptr);

	// Handles camera inputs
	void Inputs(GLFWwindow* window);
};
#endif