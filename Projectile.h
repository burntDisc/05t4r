#pragma once
#include "GameObject.h"
class Projectile :
    public GameObject
{
public:
	Projectile(
		glm::vec3 initOrientation,
		const char* modelFile,
		glm::vec3 initTranslation,
		glm::vec3 initScale,
		glm::vec3 initModelOrientation = glm::vec3(0.0f,0.0f,0.0f));
	
	void Update();

private:
	glm::vec3 modelOrientation;
	glm::vec3 orientation;
	const float speed = 40.0;


};

