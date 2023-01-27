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
	const float speed = 0.3f;
	NetworkHandler::Gamestate state = NetworkHandler::GetGamestate();

	if (state.valid)
	{
		destination = state.position;
	}

	glm::vec3 direction = destination - translation;
	
	if (length(direction))
	{
		velocity = normalize(direction) * speed;

		if (glm::length(direction) < glm::length(velocity))
		{
			translation = destination;
		}
		else
		{
			translation += velocity;
		}

	}
}

