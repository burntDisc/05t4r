#include "MotionHandler.h"

std::vector<CollisionMesh> MotionHandler::collisionMeshes;

void MotionHandler::AddSolidObject(SolidObject object)
{
	collisionMeshes.push_back(object.collisionMesh);
}

glm::vec3 MotionHandler::ApplyTranslation(glm::vec3 position, glm::vec3 destination)
{
	glm::vec3 adjustedPositionsSum(0.0, 0.0, 0.0);
	float numAdjustedPositions = 0.0;
	for (int meshIndex = 0; meshIndex < collisionMeshes.size(); ++meshIndex)
	{
		CollisionMesh& mesh = collisionMeshes[meshIndex];
		glm::vec3 adjustedPosition = mesh.GetAdjustedDestination(position, destination);
		if (adjustedPosition != destination)
		{
			adjustedPositionsSum += adjustedPosition;
			++numAdjustedPositions;
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
