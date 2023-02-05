#include "ProjectileStream.h"
#include "NetworkHandler.h"
#include <iostream>

ProjectileStream::ProjectileStream(
	const char* modelFile,
	glm::vec3 initScale,
	glm::vec3 initModelOrientation
):
	modelOrientation(initModelOrientation),
	GameObject(
		modelFile,
		glm::vec3(-9999.0f, -9999.0f, -9999.0f), // TODO: setting offscreen position... likely more elegant solution involving modifying Gameobject 
		initScale)
{
}

glm::quat ProjectileStream::GetRotation(glm::vec3 newOrientation)
{
	if (newOrientation == modelOrientation)
	{
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else if (newOrientation == -modelOrientation)
	{
		return glm::vec3(0.0f, 0.0f, 2 * acos(0.0));
	}
	else
	{
		// TODO: make math clearer
		//https://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another
		glm::quat rotation = glm::cross(modelOrientation, newOrientation);
		rotation.w = 
			(float)sqrt(
				pow(glm::length(modelOrientation), 2) * 
				pow(glm::length(newOrientation), 2)
			) + 
			glm::dot(modelOrientation, newOrientation);
		return rotation;
	}
}

void ProjectileStream::Fire(glm::vec3 newTranslation, glm::vec3 newOrientation, float* intensity)
{
	if (currentTime - prevFireTime > fireInterval && *intensity > -0.5f)
	{
		prevFireTime = currentTime;
		Projectile newProjectile;
		newProjectile.translation = newTranslation;
		newProjectile.orientation = newOrientation;
		newProjectile.rotation = GetRotation(newOrientation);

		projectiles.push_back(newProjectile);
		if (projectiles.size() > maxProjectiles) {
			projectiles.pop_front();
		}

		bool firing = true;
		NetworkHandler::SetGamestate(NetworkHandler::firing, &firing);
	}
	else
	{
		bool notFiring = false;
		NetworkHandler::SetGamestate(NetworkHandler::firing, &notFiring);

	}

}

void ProjectileStream::Draw(Shader shader)
{
	for (int i = 0; i < projectiles.size(); ++i)
	{
		model.Draw(shader, projectiles[i].translation, projectiles[i].rotation, scale);
	}
}

void ProjectileStream::Update(float time)
{
	currentTime = time;
	for (int i = 0; i < projectiles.size(); ++i)
	{
		projectiles[i].translation += projectiles[i].orientation * speed;
	}
}
