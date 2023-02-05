#pragma once

#include "GameObject.h"
#include "ProjectileStream.h"

class Opponent :
	public GameObject
{
public:
	Opponent(
		const char* modelFile,
		ProjectileStream& projectileStream,
		glm::vec3 initTranslation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 initScale = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat initRotation = glm::vec3(0.0f, 0.0f, 0.0f));

	void Update(float time);
	glm::vec3 GetPosition();
private:
	bool firing;
	float firingIntensity;
	float prevStateTime;
	float latency;
	glm::quat LookRotation(glm::vec3 orientation);
	glm::quat modelRotation;
	glm::vec3 prevTranslation;
	glm::vec3 nextTranslation;
	glm::vec3 orientation;
	ProjectileStream& projectileStream;
};

