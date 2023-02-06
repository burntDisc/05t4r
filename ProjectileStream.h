#pragma once
#include "GameObject.h"

#include <deque>

class ProjectileStream:
    public GameObject
{
public:
	ProjectileStream(
		const char* modelFile,
		glm::vec3 initScale,
		glm::vec3 initModelOrientation);
	
	void Update(float time);
	void Draw(Shader shader);

	void Fire(glm::vec3 newTranslation, glm::vec3 newOrientation, float* intensity);

	bool CheckCollision(glm::vec3 position);

private:
	float contactThreshold = 5.0f;

	struct Projectile
	{
		glm::quat rotation;
		glm::vec3 orientation;
		glm::vec3 translation;
		float intensity;
	};
	
	std::deque<Projectile> projectiles;
	glm::vec3 modelOrientation;
	glm::quat GetRotation(glm::vec3 newOrientation);

	const float maxLaunchOffset = 15.0f;
	const float minLaunchOffset = 8.0f;
	const float speed = 30.0f;
	const int maxProjectiles = 10;


};

