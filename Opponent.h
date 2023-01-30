#pragma once

#include "GameObject.h"

class Opponent :
	public GameObject
{
public:
	Opponent(
		const char* modelFile,
		glm::vec3 initTranslation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 initScale = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat initRotation = glm::vec3(0.0f, 0.0f, 0.0f));

	void Update();
private:
	glm::quat LookRotation(glm::vec3 orientation);
	glm::quat modelRotation;
};

