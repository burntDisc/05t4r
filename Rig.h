#pragma once
#include "Model.h"
#include "Shader.h"

class Rig
{
public:
	struct RiggedModel
	{
		Model model;
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
	};

	Rig(glm::vec3 initTranslation, glm::quat initRotation, glm::vec3 initScale);

	void AddModel(RiggedModel model);

	void Draw(Shader& shader);

	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
protected:
	std::vector<RiggedModel> riggedModels;
};

