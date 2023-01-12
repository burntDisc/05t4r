#include "MotionHandler.h"

std::vector<CollisionMesh> MotionHandler::collisionMeshes;

void MotionHandler::AddSolidObject(SolidObject object)
{
	collisionMeshes.push_back(object.collisionMesh);
}

glm::vec3 MotionHandler::ApplyTranslation(glm::vec3 start, glm::vec3 destination, glm::vec3&  normal)
{
	for (int meshIndex = 0; meshIndex < collisionMeshes.size(); ++meshIndex)
	{
		CollisionMesh& mesh = collisionMeshes[meshIndex];
		normal = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 adjustedPosition = mesh.GetAdjustedDestination(start, destination, normal);
		if (normal != glm::vec3(0.0,0.0,0.0))
		{
			return adjustedPosition;
		}
	}
	return destination;
}
