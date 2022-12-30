#pragma once
#include"Model.h"

class GameObject
{
public:
	GameObject(
		const char* modelFile, 
		glm::vec3 initTranslation = glm::vec3(0),
		glm::vec3 initRotation = glm::vec3(0),
		glm::vec3 initScale = glm::vec3(0));

	void Draw(Shader& shader);

	void Update(float time);

	std::vector<Mesh>& getMeshes();
protected:
	Model model;
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
};