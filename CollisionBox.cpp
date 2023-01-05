#include "CollisionBox.h"

void CollisionBox::Set(Model model, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	std::vector<glm::vec3> vertexPositions;
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
			vertexPositions.push_back(vertexWorldPosition);
		}
	}

	xMax = vertexPositions[0].x;
	xMin = vertexPositions[0].x;
	yMax = vertexPositions[0].y;
	yMin = vertexPositions[0].y;
	zMax = vertexPositions[0].z;
	zMin = vertexPositions[0].z;

	// Finding bounds in world space
	for (int vertexIndex = 0; vertexIndex < vertexPositions.size(); ++vertexIndex)
	{
		glm::vec3& vertexPosition = vertexPositions[vertexIndex];
		if (vertexPosition.x > xMax)
		{
			xMax = vertexPosition.x;
		}
		if (vertexPosition.y > yMax)
		{
			yMax = vertexPosition.y;
		}
		if (vertexPosition.z > zMax)
		{
			zMax = vertexPosition.z;
		}
		if (vertexPosition.x < xMin)
		{
			xMin = vertexPosition.x;
		}
		if (vertexPosition.y < yMin)
		{
			yMin = vertexPosition.y;
		}
		if (vertexPosition.z < zMin)
		{
			zMin = vertexPosition.z;
		}
	}
}

bool CollisionBox::CheckCollison(glm::vec3 position)
{
	bool withinXBounds = position.x < xMax && position.x > xMin;
	bool withinYBounds = position.y < yMax && position.y > yMin;
	bool withinZBounds = position.z < zMax && position.z > zMin;
	return withinXBounds && withinYBounds && withinZBounds;
}
