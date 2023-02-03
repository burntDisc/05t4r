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
		glm::vec3 initModelOrientation = glm::vec3(0.0f,0.0f,0.0f));
	
	void Update(float time);
	void Draw(Shader shader);

	void Fire(glm::vec3 newTranslation, glm::vec3 newOrientation, float* intensity = nullptr);

	struct Projectile
	{
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 orientation;
	};
	static const int maxProjectiles = 10;
private:
	float currentTime = 0;
	const float fireInterval = 0.1f;
	float prevFireTime = 0;
	
	std::deque<Projectile> projectiles;
	glm::quat GetRotation(glm::vec3 newOrientation);
	glm::vec3 modelOrientation;
	const float speed = 70.0f;


};

