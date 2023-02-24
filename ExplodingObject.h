#pragma once

#include "GameObject.h"
#include "Shader.h"

class ExplodingObject :
	public GameObject
{
public:
	ExplodingObject(
		const char* modelFile = nullptr,
		glm::vec3 initTranslation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 initScale = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat initRotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 modelOffset = glm::vec3(0.0f, 0.0f, 0.0f));
	void Update(double time);
	void Draw(Shader& shader);
protected:
	float phase;
	float amplitude = 0.5f;
	float speed = 0.5f;
};

