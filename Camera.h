#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "InputHandler.h"
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
	float speed = 0.5f;
	float sensitivity = 100.0f;

	Camera(int width, int height, glm::vec3 position);

	// Exports the POV related unifroms
	void SetCameraUniforms(Shader& shader);
	void Forward();// TODO_SOON move input handler
	void TranslateLeft();
	void Back();
	void TranslateRight();
	void TranslateUp();

	// Handles camera inputs
	void Inputs(GLFWwindow* window);
};