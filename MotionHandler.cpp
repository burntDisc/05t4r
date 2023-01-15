#include "MotionHandler.h"
#include <glm/gtx/projection.hpp>

std::vector<GameObject*> MotionHandler::solidObjects;
static const float minTravelLength = 0.0000001f;

void MotionHandler::AddSolidObject(GameObject* object)
{
	solidObjects.push_back(object);
}

float MotionHandler::SignOfQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	float quadVolume = glm::dot(glm::cross(b - a, c - a), d - a);
	return glm::sign(quadVolume);
}

//adjusts destination to avoid passing through triangles
glm::vec3 MotionHandler::ApplyTranslation(glm::vec3 start, glm::vec3 destination, glm::vec3&  normal)
{
	glm::vec3 newDestination = destination;
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int objectIndex = 0; objectIndex < solidObjects.size(); ++objectIndex)
	{
		GameObject* object = solidObjects[objectIndex];
		normal = glm::vec3(0.0, 0.0, 0.0);
		float depth = 1.50;
		float numLayers = 6;
		float buffer = 0.5;
		glm::vec3 travel = destination - start;
		float lengthOfTravel = glm::length(travel);

		// This check prevents slow crawl through triangle via float innacuracies
		if (lengthOfTravel > minTravelLength)
		{
			std::vector<glm::vec3> objectTrianglePositions = object->GetTriangleWorldPositions();
			std::vector<glm::vec3> objectTriangleNormals = object->GetTriangleWorldNormals();

			for (int triangleIndex = 0; triangleIndex < objectTrianglePositions.size(); triangleIndex += 3)
			{
				// getting normals
				glm::vec3 vertexNormalA = objectTriangleNormals[triangleIndex + 0];
				glm::vec3 vertexNormalB = objectTriangleNormals[triangleIndex + 1];
				glm::vec3 vertexNormalC = objectTriangleNormals[triangleIndex + 2];

				glm::vec3 triangleNormal = normalize((vertexNormalA + vertexNormalB + vertexNormalC) / 3.0f);

				// here I adjust the recognized triangle position outward to along normals avoid clipping
				glm::vec3 modelVertexPositionA = objectTrianglePositions[triangleIndex + 0] + buffer * normalize(vertexNormalA);
				glm::vec3 modelVertexPositionB = objectTrianglePositions[triangleIndex + 1] + buffer * normalize(vertexNormalB);
				glm::vec3 modelVertexPositionC = objectTrianglePositions[triangleIndex + 2] + buffer * normalize(vertexNormalC);

				glm::vec3 trianglePosition = normalize((modelVertexPositionA + modelVertexPositionB + modelVertexPositionC) / 3.0f);

				// Checks for shell of triangles translated along normals to handle overshoot
				// TODO: possibly better way to handle this with less overhead.
				for (float layer = 0.0f; layer < depth; layer += depth / numLayers)
				{
					glm::vec3 vertexPositionA = modelVertexPositionA - layer * normalize(vertexNormalA);
					glm::vec3 vertexPositionB = modelVertexPositionB - layer * normalize(vertexNormalB);
					glm::vec3 vertexPositionC = modelVertexPositionC - layer * normalize(vertexNormalC);

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
	}
	return newDestination;
}
