#pragma once
#include "GameObject.h"
#include "CollisionBox.h"

class SolidObject :
    public GameObject
{
public:
	SolidObject(
		const char* modelFile,
		glm::vec3 initTranslation = glm::vec3(0),
		glm::quat initRotation = glm::quat(0.0, 0.0, 0.0, 0.0),
		glm::vec3 initScale = glm::vec3(0));
	bool CheckCollison(glm::vec3 position);
private:
	CollisionBox collisionBox;
};

