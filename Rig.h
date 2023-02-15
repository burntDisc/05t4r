#pragma once
#include "Model.h"
#include "Shader.h"
#include <cstdarg>

class Rig
{
public:
	struct RiggedModel
	{
		Model model;
		glm::vec3 translationOffset;
	};

	Rig(int numModels,...);
	void Draw
	(
		Shader& shader,
		glm::vec3 translation,
		glm::quat rotation,
		glm::vec3 scale
	);
private:
	std::vector<RiggedModel> riggedModels;
};

