#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Shader.h"
#include "Model.h"

class GameObject
{
public:
	GameObject(
		const char* modelFile, 
		glm::vec3 initTranslation = glm::vec3(0),
		glm::quat initRotation = glm::quat(0.0, 0.0, 0.0, 0.0),
		glm::vec3 initScale = glm::vec3(0));

	void Draw(Shader& shader);

	void UpdateWorldVertices();

	std::vector<glm::vec3> GetVertexWorldNormals();
	std::vector<glm::vec3> GetVertexWorldPositions();
	std::vector<glm::vec3> GetTriangleWorldNormals();
	std::vector<glm::vec3> GetTriangleWorldPositions();

	Model model;
protected:
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	std::vector<glm::vec3> vertexWorldNormals;
	std::vector<glm::vec3> vertexWorldPositions;
	std::vector<glm::vec3> triangleWorldNormals;
	std::vector<glm::vec3> triangleWorldPositions;
	bool validWorldPositions;
};