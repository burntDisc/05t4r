#include "MotionHandler.h"

std::vector<CollisionMesh> MotionHandler::collisionMeshes;

void MotionHandler::AddSolidObject(SolidObject object)
{
	collisionMeshes.push_back(object.collisionMesh);
}

glm::vec3 MotionHandler::ApplyTranslation(glm::vec3 start, glm::vec3 destination, glm::vec3&  normal)
{
	glm::vec3 adjustedPositionsSum(0.0, 0.0, 0.0);
	float numAdjustedPositions = 0.0;
	float traversalDistance = glm::length(destination - start);
	for (int meshIndex = 0; meshIndex < collisionMeshes.size(); ++meshIndex)
	{
		CollisionMesh& mesh = collisionMeshes[meshIndex];
		float meshDistance = glm::length(mesh.centerPoint - start);
		if (meshDistance < traversalDistance + mesh.radius)
		{
			glm::vec3 adjustedPosition = mesh.GetAdjustedDestination(start, destination, normal);
			if (adjustedPosition != destination)
			{
				adjustedPositionsSum += adjustedPosition;
				++numAdjustedPositions;
			}
			else
			{
				normal = glm::vec3(0.0, 0.0, 0.0);
			}
		}
	}
	if (numAdjustedPositions > 0)
	{
		return adjustedPositionsSum / numAdjustedPositions;
	}
	else
	{
		return destination;
	}
}
