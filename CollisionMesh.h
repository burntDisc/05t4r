#pragma once

#include <glm/glm.hpp>

#include "Model.h"

class CollisionMesh
{
public:
	void SetMesh(Model model, glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
	glm::vec3 GetAdjustedDestination(glm::vec3 position, glm::vec3 destination, glm::vec3& normal);
	glm::vec3 centerPoint;
	float radius;
private:
	void PushBounds(glm::vec3 position);
	float signed_tetra_volume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexNormals;
	std::vector<GLuint> vertexIndices;
	glm::vec3 maxBounds;
	glm::vec3 minBounds;
};

