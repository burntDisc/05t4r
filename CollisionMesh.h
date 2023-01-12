#pragma once

#include <glm/glm.hpp>

#include "Model.h"

class CollisionMesh
{
public:
	void SetMesh(Model model, glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
	glm::vec3 GetAdjustedDestination(glm::vec3 position, glm::vec3 destination, glm::vec3& normal);
private:
	float SignOfQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexNormals;
	std::vector<GLuint> vertexIndices;
	const float minTravelLength = 0.0000001f;
};

