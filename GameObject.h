#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Shader.h"
#include "Model.h"

class GameObject
{
public:
	GameObject(
		const char* modelFile, 
		glm::vec3 initTranslation = glm::vec3(0),
		glm::quat initRotation = glm::quat(0.0, 0.0, 0.0, 0.0),
		glm::vec3 initScale = glm::vec3(0));

	void Draw(Shader& shader);

	Model model;
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
};