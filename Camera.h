#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(int windowWidth, int windowHeight, glm::vec3* translation, glm::vec3* orientation, float* feildOfView);

	// Exports the POV related unifroms
	void SetCameraUniforms(Shader& shader);
	void SetSkyboxUniforms(Shader& skyboxShader);
private:
	float* feildOfView;
	const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	int windowWidth, windowHeight;
	glm::vec3* translation;
	glm::vec3* orientation;
};

