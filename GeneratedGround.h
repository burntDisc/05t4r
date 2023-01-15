#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "GameObject.h"

class GeneratedGround :
	public GameObject
{
public:
	GeneratedGround(
		Camera* camera,
		const char* modelFile,
		glm::vec3 initTranslation = glm::vec3(0),
		glm::quat initRotation = glm::quat(0.0, 0.0, 0.0, 0.0),
		glm::vec3 initScale = glm::vec3(0));

	void Draw(Shader& shader);
private:
	Camera* camera;
};