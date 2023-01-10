#pragma once

#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "CollisionMesh.h"
#include "SolidObject.h"

class MotionHandler
{
public:
	static void AddSolidObject(SolidObject object);
	static glm::vec3 ApplyTranslation(glm::vec3 position, glm::vec3 translation);
private:
	static std::vector<CollisionMesh> collisionMeshes;
};


