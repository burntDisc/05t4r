#include "Opponent.h"
#include "NetworkHandler.h"

Opponent::Opponent
(
	const char* modelFile,
	glm::vec3 initTranslation,
	glm::vec3 initScale,
	glm::quat initRotation
) :
	GameObject(
		modelFile,
		initTranslation,
		initScale,
		initRotation)
{
}

void Opponent::Update()
{
	NetworkHandler::Gamestate state = NetworkHandler::GetGamestate();
	translation = state.position;
}

