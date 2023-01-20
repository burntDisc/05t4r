#include "Projectile.h"

Projectile::Projectile(
	glm::vec3 initOrientation, 
	const char* modelFile, 
	glm::vec3 initTranslation, 
	glm::vec3 initScale,
	glm::vec3 initModelOrientation
):
	modelOrientation(initModelOrientation),
	orientation(initOrientation),
	GameObject(
		modelFile,
		initTranslation,
		initScale)
{
	if (orientation == modelOrientation)
	{

	}
	else if(orientation == -modelOrientation)
	{
		rotation = glm::vec3(0.0f, 0.0f, 2 * acos(0.0));
	}
	else
	{
		glm::vec3 a = glm::cross(modelOrientation, initOrientation);
		rotation = a;
		rotation.w = sqrt(pow(glm::length(modelOrientation), 2) * pow(glm::length(initOrientation), 2)) + glm::dot(modelOrientation, initOrientation);
	}
}

void Projectile::Update()
{
	translation += orientation * speed;
}
