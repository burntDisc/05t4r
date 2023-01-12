#include "CollisionMesh.h"

#include <glm/gtx/projection.hpp>
#include <glm/gtx/vector_angle.hpp>

void CollisionMesh::SetMesh(Model  model, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	// converting transformations to mat4 for combination
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
			// Apply transformation to get world position
			// TODO: There might be a better way to do this (copied from shader)
			glm::vec3 vertexWorldPosition(meshPos * translationMatrix * rotationMatrix * scaleMatrix * glm::vec4(vertex.position, 1.0f));
			glm::vec3 vertexWorldNormal(rotationMatrix * glm::vec4(vertex.normal, 1.0f));
			vertexPositionSum += vertexWorldPosition;
			vertexPositions.push_back(vertexWorldPosition);
			vertexNormals.push_back(normalize(vertexWorldNormal));
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

float CollisionMesh::SignOfQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	float quadVolume = glm::dot(glm::cross(b - a, c - a), d - a);
	return glm::sign(quadVolume);
}

glm::vec3 CollisionMesh::GetAdjustedDestination(glm::vec3 start, glm::vec3 destination, glm::vec3& normal)
{

	float depth = 1.50;
	float numLayers = 6;
	float buffer = 0.5;
	glm::vec3 newDestination = destination;
	glm::vec3 travel = destination - start;
	float lengthOfTravel = glm::length(travel);

	// This check prevents slow crawl through trianglie via float innacuracies
	if (lengthOfTravel > minTravelLength)
	{
		for (int triangleIndex = 0; triangleIndex < vertexIndices.size(); triangleIndex += 3)
		{
			GLuint vertexIndiceA = vertexIndices[triangleIndex];
			GLuint vertexIndiceB = vertexIndices[triangleIndex + 1];
			GLuint vertexIndiceC = vertexIndices[triangleIndex + 2];

			glm::vec3 vertexNormalA = vertexNormals[vertexIndiceA];
			glm::vec3 vertexNormalB = vertexNormals[vertexIndiceB];
			glm::vec3 vertexNormalC = vertexNormals[vertexIndiceC];

			glm::vec3 triangleNormal = normalize((vertexNormalA + vertexNormalB + vertexNormalC) / 3.0f);

			glm::vec3 trianglePosition = normalize((vertexPositions[vertexIndiceA] + vertexPositions[vertexIndiceB] + vertexPositions[vertexIndiceC])/3.0f + buffer * normalize(triangleNormal));

			// Checks for shell of triangles translated along normals to handle overshoot
			// TODO: possibly better way to handle this with less overhead.
			for (float layer = 0.0f; layer < depth; layer += depth / numLayers)
			{
				glm::vec3 vertexPositionA = vertexPositions[vertexIndiceA] - layer * normalize(vertexNormalA) + buffer * normalize(vertexNormalA);
				glm::vec3 vertexPositionB = vertexPositions[vertexIndiceB] - layer * normalize(vertexNormalB) + buffer * normalize(vertexNormalB);
				glm::vec3 vertexPositionC = vertexPositions[vertexIndiceC] - layer * normalize(vertexNormalC) + buffer * normalize(vertexNormalC);

				// check for plane traversal
				float s1 = SignOfQuad(start, vertexPositionA, vertexPositionB, vertexPositionC);
				float s2 = SignOfQuad(destination, vertexPositionA, vertexPositionB, vertexPositionC);

				if (s1 != s2)
				{
					//check for triangle traversal
					float s3 = SignOfQuad(start, destination, vertexPositionA, vertexPositionB);
					float s4 = SignOfQuad(start, destination, vertexPositionB, vertexPositionC);
					float s5 = SignOfQuad(start, destination, vertexPositionC, vertexPositionA);
					if (s3 == s4 && s4 == s5)
					{
						float approachSkew = glm::dot(trianglePosition - start, triangleNormal);
						float travelSkew = glm::dot(travel, triangleNormal);
						glm::vec3 adjustment;

						//converting close-to-plane collisions as parrallel to plane paths
						if (travelSkew < 0.00001) {
							adjustment = travel - glm::proj(travel, triangleNormal);
						}
						else
						{
							// adjust travel to buffered triangle face
							float t = approachSkew / travelSkew;
							adjustment = t * travel;
						}

						glm::vec3 possibleDestination = start + adjustment;

						// use shortest path (edge cases handled by buffer adjustments)
						// TODO: possibly better way to handle edge cases that causes tunneling at triangle edges
						if (glm::length(start - newDestination) > glm::length(start - possibleDestination))
						{
							newDestination = possibleDestination;
							normal = triangleNormal;
						}
					}
				}
			}
		}
	}
	return newDestination;
}
