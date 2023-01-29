#include "Opponent.h"
#include "NetworkHandler.h"

#include <iostream>

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
{}

void Opponent::Update()
{
	NetworkHandler::Gamestate state = NetworkHandler::GetGamestate();

	if (state.valid)
	{
		translation = state.position;

		rotation = glm::lookAt(
			(glm::vec3)0,				// position
			state.orientation,			// target
			glm::vec3(0.0f, 1.0f, 0.0f) // up
		);
	}
}

