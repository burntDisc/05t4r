
#pragma once

#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "GameObject.h"

class MotionHandler
{
public:
	static void AddSolidObject(GameObject* object);
private:
	static float SignOfQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	static std::vector<GameObject*> solidObjects;

	class PLANE {
	public:
		float equation[4];
		glm::vec3 origin;
		glm::vec3 normal;
		PLANE(const glm::vec3& origin, const glm::vec3& normal);
		PLANE(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& normal);
		bool isFrontFacingTo(const glm::vec3& direction) const;
		float SignedDistanceTo(const glm::vec3& point) const;
	};

	struct CollisionPacket {
		glm::vec3 eRadius = glm::vec3(1.0f, 1.0f, 1.0f); // ellipsoid radius
		// Information about the move being requested: (in R3)
		glm::vec3 R3Velocity;
		glm::vec3 R3Position;
		// Information about the move being requested: (in eSpace)
		glm::vec3 velocity;
		glm::vec3 normalizedVelocity;
		glm::vec3 basePoint;
		// Hit information
		bool foundCollision;
		float nearestDistance;
		glm::vec3 intersectionPoint;
	};

	static void CheckTriangle(CollisionPacket* colPackage,
		const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& triNormal, glm::vec3& normal);

	static glm::vec3 CollideWithWorld(const glm::vec3& pos, const glm::vec3& vel,
		glm::vec3& normal, int collisionRecursionDepth = 0);


	static CollisionPacket packet;
public:

	static glm::vec3 CheckCollision(CollisionPacket* collisionPackage, glm::vec3& normal);
	static glm::vec3 CollideAndSlide(const glm::vec3& position, const glm::vec3& vel, glm::vec3& normal);
};


