#include "SolidObject.h"
#include <iostream>
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
	collisionBox.Set(model, initTranslation, initRotation, initScale);
}

bool SolidObject::CheckCollison(glm::vec3 position)
{
	return collisionBox.CheckCollison(position);
}
