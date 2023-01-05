#include "CollisionMesh.h"
/*
#include <iostream>
*/
void CollisionMesh::SetMesh(Model  model, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	translationMatrix = glm::translate(translationMatrix, translation);
	rotationMatrix = glm::mat4_cast(rotation);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	std::vector<Mesh>& meshes = model.GetMeshes();
	std::vector<glm::mat4>& meshTransforms = model.GetMeshTransforms();

	// collecting vertices from all meshes
	for (int meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
	{
		glm::mat4& meshPos = meshTransforms[meshIndex];
		std::vector<Vertex>& vertices = meshes[meshIndex].vertices;
		for (int vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex)
		{
			Vertex& vertex = vertices[vertexIndex];
			glm::vec3 vertexWorldPosition(meshPos * translationMatrix * rotationMatrix * scaleMatrix * glm::vec4(vertex.position, 1.0f));
			glm::vec3 vertexWorldNormal(meshPos * translationMatrix * rotationMatrix * scaleMatrix * glm::vec4(vertex.position, 1.0f));
			vertexPositions.push_back(vertexWorldPosition);
			vertexNormals.push_back(normalize(vertexWorldNormal));
		}
	}
}

glm::vec3 CollisionMesh::CheckCollison(glm::vec3 position, float threshold)
{
	float numberColliding = 0.0;
	glm::vec3 normalSum(0.0, 0.0, 0.0);
	glm::vec3 averagedNormal(0.0, 0.0, 0.0);
	for (int vertexIndex = 0; vertexIndex < vertexPositions.size(); ++vertexIndex)
	{
		glm::vec3 vertexPosition = vertexPositions[vertexIndex];
		float distance = glm::length(vertexPosition - position);
		if (distance < threshold) {
			normalSum += vertexNormals[vertexIndex];
			++numberColliding;
		}
	}
	if (normalSum != glm::vec3(0.0, 0.0, 0.0))
	{
		averagedNormal = normalSum / numberColliding;
	}
	return averagedNormal;
}
