#pragma once
#include "ExplodingObject.h"
#include "Updatable.h"
#include "Drawable.h"

#include <deque>

class ProjectileStream:
    public ExplodingObject,
	public Updatable,
	public Drawable
{
public:
	ProjectileStream(
		Shader shader,
		Shader secondShader,
		const char* modelFile,
		glm::vec3 initScale,
		glm::vec3 initModelOrientation,
		const char* secondModelFile = nullptr);
	
	void Update(double time);
	void Draw();

	void Fire(glm::vec3 newTranslation, glm::vec3 newOrientation, float* intensity);

	bool CheckCollision(glm::vec3 position);

private:
	Shader secondShader;
	Model secondModel;

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

