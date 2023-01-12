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
	return glm::sign(glm::dot(glm::cross(b - a, c - a), d - a) / 6.0);
}

glm::vec3 CollisionMesh::GetAdjustedDestination(glm::vec3 start, glm::vec3 destination, glm::vec3& normal)
{
	glm::vec3 travel = destination - start;
	float lengthOfTravel = glm::length(travel);

	float depth = 1.50;
	float numLayers = 6;
	glm::vec3 destinationSum(0.0f, 0.0f, 0.0f);
	glm::vec3 normalSum(0.0f, 0.0f, 0.0f);
	float numFound = 0.0f;
	float buffer = 0.5;
	float t = 0.0;
	float numerator = 0.0;
	float denominator = 0.0;
	glm::vec3 returnVal = destination;
	if (lengthOfTravel > 0.000001)
	{
		for (int triangleIndex = 0; triangleIndex < vertexIndices.size(); triangleIndex += 3)
		{
			GLuint vertexIndiceA = vertexIndices[triangleIndex];
			GLuint vertexIndiceB = vertexIndices[triangleIndex + 1];
			GLuint vertexIndiceC = vertexIndices[triangleIndex + 2];

			glm::vec3 vertexNormalA = vertexNormals[vertexIndiceA];
			glm::vec3 vertexNormalB = vertexNormals[vertexIndiceB];
			glm::vec3 vertexNormalC = vertexNormals[vertexIndiceC];

			glm::vec3 triangleNormal = normalize( (vertexNormalA + vertexNormalB + vertexNormalC) / 3.0f);

			for (float layer = 0.0f; layer < depth; layer+=depth/numLayers) 
			{
				glm::vec3 vertexPositionA = vertexPositions[vertexIndiceA] - layer * normalize(vertexNormalA) + buffer * normalize(vertexNormalA);
				glm::vec3 vertexPositionB = vertexPositions[vertexIndiceB] - layer * normalize(vertexNormalB) + buffer * normalize(vertexNormalB);
				glm::vec3 vertexPositionC = vertexPositions[vertexIndiceC] - layer * normalize(vertexNormalC) + buffer * normalize(vertexNormalC);


				//----------------------------------------

				//formula modified from: https://stackoverflow.com/questions/42740765/intersection-between-line-and-triangle-in-3d

		
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
						numerator = glm::dot(vertexPositionA - start, triangleNormal);
						denominator = glm::dot(travel, triangleNormal);
						glm::vec3 adjustment;
						if (denominator < 0.00001) {
							adjustment = travel - glm::proj(travel, triangleNormal);
						}
						else
						{
							t = numerator / denominator; //float t = glm::dot(vertexPositionA - start, triangleNormal) / glm::dot(travel, triangleNormal);
							adjustment = (t < 0.0001 || t > 9999999) ? glm::vec3(0.0, 0.0, 0.0) : t * travel;
						}
							destinationSum += start + adjustment;
							normalSum += triangleNormal;
							++numFound;
					}

				}
			}
		}
		if (numFound > 0.0)
		{
			normal = normalize(normalSum / numFound);
			returnVal = destinationSum / numFound;
		}
		else
		{
			normal = glm::vec3(0.0, 0.0, 0.0);
			returnVal = destination;
		}
		if (glm::length(returnVal - start) > 10.0f)
		{
 			std::cout << "JUMP!"  << std::endl;
			std::cout << "newPosition: " << std::endl;
			std::cout << returnVal.x << std::endl;
			std::cout << returnVal.y << std::endl;
			std::cout << returnVal.z << std::endl;
			std::cout << "sum: " << std::endl;
			std::cout << destinationSum.x << std::endl;
			std::cout << destinationSum.y << std::endl;
			std::cout << destinationSum.z << std::endl;
			std::cout << "num: " << numFound << std::endl;
			std::cout << "t: " << t << std::endl;
			std::cout << "travel: " << std::endl;
			std::cout << travel.x << std::endl;
			std::cout << travel.y << std::endl;
			std::cout << travel.z << std::endl;
			std::cout << "numerator: " << numerator << std::endl;
			std::cout << "denominator: " << denominator << std::endl;
			glm::vec3  testValue = destinationSum / numFound;
		}

	}
	return returnVal;
}
