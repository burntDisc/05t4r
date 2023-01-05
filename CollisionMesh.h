#pragma once

#include <glm/glm.hpp>

#include "Model.h"

class CollisionMesh
{
public:
	void SetMesh(Model model, glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
	glm::vec3 CheckCollison(glm::vec3 position, float threshold = 30.0);
private:
	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexNormals;
};

