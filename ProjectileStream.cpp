#include "ProjectileStream.h"
#include "NetworkHandler.h"
#include <iostream>
#include <glm/gtx/projection.hpp>

ProjectileStream::ProjectileStream(
	Shader shader,
	Shader secondShader,
	const char* modelFile, 
	glm::vec3 initScale, 
	glm::vec3 initModelOrientation, 
	const char* secondModelFile ) :

	Drawable(shader),
	secondShader(secondShader),
	modelOrientation(initModelOrientation),
	ExplodingObject(
		modelFile,
		glm::vec3(-9999.0f, -9999.0f, -9999.0f), // TODO: setting offscreen position... likely more elegant solution involving modifying Gameobject 
		initScale),
	secondModel(secondModelFile)
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
	float launchOffset = minLaunchOffset + (maxLaunchOffset - minLaunchOffset) * (float)rand() / (float)RAND_MAX;
	Projectile newProjectile = {};
	newProjectile.rotation = GetRotation(newOrientation);
	newProjectile.orientation = newOrientation;
	newProjectile.translation =  newTranslation + launchOffset * newOrientation;
	newProjectile.intensity = *intensity;

	projectiles.push_back(newProjectile);
	if (projectiles.size() > maxProjectiles) 
	{
		projectiles.pop_front();
	}
}

bool ProjectileStream::CheckCollision(glm::vec3 position)
{
	for (int i = 0; i < projectiles.size(); ++i)
	{
		glm::vec3 travel = projectiles[i].translation - position;
		glm::vec3 travelComponent = glm::proj(travel, projectiles[i].orientation);
		glm::vec3 radialComponent = travel - travelComponent;
		if (glm::length(radialComponent) < contactThreshold)
		{
			if (glm::length(travel) < speed * (projectiles[i].intensity + 1.0f))
			{
				return true;
			}
		}

	}
	return false;
}

void ProjectileStream::Delete()
{
	delete this;
}

void ProjectileStream::Draw()
{
	shader.Activate();
	glUniform1fv(glGetUniformLocation(shader.ID, "phase"), 1, &phase);
	glUniform1fv(glGetUniformLocation(shader.ID, "amplitude"), 1, &amplitude);
	glUniform1fv(glGetUniformLocation(shader.ID, "speed"), 1, &speed);
	glUniform1i(glGetUniformLocation(shader.ID, "explode"), true);
	for (int i = 0; i < projectiles.size(); ++i)
	{
		model.Draw(shader, projectiles[i].translation, projectiles[i].rotation, scale);
		secondModel.Draw(secondShader, projectiles[i].translation, projectiles[i].rotation, scale);
	}
}

void ProjectileStream::Update(double time)
{
	Updatable::Update(time);
	for (int i = 0; i < projectiles.size(); ++i)
	{
		projectiles[i].translation += projectiles[i].orientation * speed * (projectiles[i].intensity + 1.0f) * (float)timeDelta;
	}
	ExplodingObject::Update(time);
}
