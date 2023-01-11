#include "CollisionMesh.h"

#include <glm/gtx/projection.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream> // REMOVE

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

	for (int meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
	{
		// collecting vertices
		glm::vec3 vertexPositionSum(0.0f,0.0f,0.0f);
		glm::mat4& meshPos = meshTransforms[meshIndex];
		std::vector<Vertex>& vertices = meshes[meshIndex].vertices;
		int startingVertexIndex = vertexPositions.size();
		for (int vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex)
		{
			Vertex& vertex = vertices[vertexIndex];
			glm::vec3 vertexWorldPosition(meshPos * translationMatrix * rotationMatrix * scaleMatrix * glm::vec4(vertex.position, 1.0f));
			glm::vec3 vertexWorldNormal(rotationMatrix * glm::vec4(vertex.normal, 1.0f));
			vertexPositionSum += vertexWorldPosition;
			vertexPositions.push_back(vertexWorldPosition);
			vertexNormals.push_back(normalize(vertexWorldNormal));
		}
		//create collision sphere for aproximation
		centerPoint = vertexPositionSum / (float)(vertexPositions.size() - startingVertexIndex);
		radius = 0.0;
		for (int vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex)
		{
			Vertex& vertex = vertices[vertexIndex];
			float distance = glm::length(centerPoint - vertex.position);
			if (distance > radius)
			{
				radius = distance;
			}
		}
		// collecting indices
		std::vector<GLuint>& indices = meshes[meshIndex].indices;
		for (int i = 0; i < indices.size(); ++i)
		{
			GLuint& indice = indices[i];
			vertexIndices.push_back(indice + startingVertexIndex);
		}
	}
}

float CollisionMesh::signed_tetra_volume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	return glm::sign(glm::dot(glm::cross(b - a, c - a), d - a) / 6.0);
}

glm::vec3 CollisionMesh::GetAdjustedDestination(glm::vec3 start, glm::vec3 destination, glm::vec3& normal)
{
	float lengthOfTravel = glm::length(start - destination);

	glm::vec3 destinationSum(0.0f, 0.0f, 0.0f);
	float numFound = 0.0f;

	for (int triangleIndex = 0; triangleIndex < vertexIndices.size(); triangleIndex += 3)
	{
		GLuint vertexIndiceA = vertexIndices[triangleIndex];
		GLuint vertexIndiceB = vertexIndices[triangleIndex + 1];
		GLuint vertexIndiceC = vertexIndices[triangleIndex + 2];
		glm::vec3 vertexNormalA = vertexNormals[vertexIndiceA];
		glm::vec3 vertexNormalB = vertexNormals[vertexIndiceB];
		glm::vec3 vertexNormalC = vertexNormals[vertexIndiceC];

		glm::vec3 vertexPositionA = vertexPositions[vertexIndiceA];
		glm::vec3 vertexPositionB = vertexPositions[vertexIndiceB];
		glm::vec3 vertexPositionC = vertexPositions[vertexIndiceC];

		glm::vec3 triangleNormal = (vertexNormalA + vertexNormalB + vertexNormalC) / 3.0f;
		glm::vec3 triangleCenter = (vertexPositionA + vertexPositionB + vertexPositionC) / 3.0f;


		//----------------------------------------

		//formula modified from: https://stackoverflow.com/questions/42740765/intersection-between-line-and-triangle-in-3d

		float s1 = signed_tetra_volume(start, vertexPositionA, vertexPositionB, vertexPositionC);
		float s2 = signed_tetra_volume(destination, vertexPositionA, vertexPositionB, vertexPositionC);

		if (s1 != s2)
		{
			float s3 = signed_tetra_volume(start, destination, vertexPositionA, vertexPositionB);
			float s4 = signed_tetra_volume(start, destination, vertexPositionB, vertexPositionC);
			float s5 = signed_tetra_volume(start, destination, vertexPositionC, vertexPositionA);
			if (s3 == s4 && s4 == s5)
			{
				float t = glm::dot(triangleCenter - start, triangleNormal) / glm::dot(destination - start, triangleNormal);
				normal = triangleNormal;
				destinationSum += start + t * (destination - start) - 0.25f * normalize(destination - start);
				++numFound;
			}
		}
	}
	if (numFound > 0.0)
	{
		return destinationSum / numFound;
	}
	return destination;
}
