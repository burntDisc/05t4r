#include "SolidObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

SolidObject::SolidObject(
	const char* modelFile, 
	glm::vec3 initTranslation, 
	glm::quat initRotation, 
	glm::vec3 initScale):

	GameObject(
		modelFile,
		initTranslation,
		initRotation,
		initScale)
{
	collisionMesh.SetMesh(GameObject::model, initTranslation, initRotation, initScale);
}

glm::vec3 SolidObject::CheckCollison(glm::vec3 position)
{
	return collisionMesh.CheckCollison(position);
}
