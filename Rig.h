#pragma once
#include "Model.h"
#include "Shader.h"
#include "Drawable.h"

class Rig:
	public Drawable
{
public:
	struct RiggedModel
	{
		Model model;
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
	};

	Rig(Shader shader, glm::vec3 initTranslation, glm::quat initRotation, glm::vec3 initScale);

	void AddModel(RiggedModel model);

	void Draw();

	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
protected:
	std::vector<RiggedModel> riggedModels;
};

