#include "GeneratedGround.h"

GeneratedGround::GeneratedGround(
	Camera* camera,
	const char* modelFile,
	glm::vec3 initTranslation,
	glm::quat initRotation,
	glm::vec3 initScale) :
	camera(camera),
	GameObject(
		modelFile,
		initTranslation,
		initRotation,
		initScale)
{
}

void GeneratedGround::Draw(Shader& shader)
{
}


