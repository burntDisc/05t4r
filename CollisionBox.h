#pragma once
#include "Model.h"

class CollisionBox
{
public:
	void Set(Model model, glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
	bool CheckCollison(glm::vec3 position);
private:
	float xMax, xMin, yMax, yMin, zMax, zMin;
};

