#pragma once

#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "GameObject.h"

class MotionHandler
{
public:
	static void AddSolidObject(GameObject* object);
	static glm::vec3 ApplyTranslation(glm::vec3 start, glm::vec3 translation, glm::vec3& normal);
private:
	static float SignOfQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	static std::vector<GameObject*> solidObjects;
};


