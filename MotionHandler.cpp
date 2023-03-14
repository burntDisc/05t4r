#include "MotionHandler.h"
#include <glm/gtx/projection.hpp>

#include <iostream> // REMOVE 

//***************************************************************
//TODO: http://www.peroxide.dk/papers/collision/collision.pdf
// **************************************************************

std::vector<GameObject*> MotionHandler::solidObjects;
MotionHandler::CollisionPacket MotionHandler::packet = {};

void MotionHandler::AddSolidObject(GameObject* object)
{
	solidObjects.push_back(object);
}

float MotionHandler::SignOfQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	float quadVolume = glm::dot(glm::cross(b - a, c - a), d - a);
	return glm::sign(quadVolume);
}

//adjusts destination to avoid passing through triangles
glm::vec3 MotionHandler::CheckCollision(MotionHandler::CollisionPacket* collisionPackage, glm::vec3& normal)
{
	glm::mat3 ellipse_Transform =
	{
		{1.0f / collisionPackage->eRadius.x, 0.0f, 0.0f},
		{0.0f, 1.0f / collisionPackage->eRadius.y, 0.0f},
		{0.0f, 0.0f, 1.0f / collisionPackage->eRadius.z}
	};

	for (int objectIndex = 0; objectIndex < solidObjects.size(); ++objectIndex)
	{
		GameObject* object = solidObjects[objectIndex];

		std::vector<glm::vec3> objectTrianglePositions = object->GetTriangleWorldPositions();
		std::vector<glm::vec3> objectTriangleNormals = object->GetTriangleWorldNormals();

		for (int triangleIndex = 0; triangleIndex < objectTrianglePositions.size(); triangleIndex += 3)
		{
			// here I adjust the recognized triangle position outward to along normals avoid clipping
			glm::vec3 modelVertexPositionA = ellipse_Transform * objectTrianglePositions[triangleIndex + 0];
			glm::vec3 modelVertexPositionB = ellipse_Transform * objectTrianglePositions[triangleIndex + 1];
			glm::vec3 modelVertexPositionC = ellipse_Transform * objectTrianglePositions[triangleIndex + 2];

			glm::vec3 modelVertexNormalA =	 ellipse_Transform * objectTriangleNormals	[triangleIndex + 0];
			glm::vec3 modelVertexNormalB =	 ellipse_Transform * objectTriangleNormals	[triangleIndex + 1];
			glm::vec3 modelVertexNormalC =	 ellipse_Transform * objectTriangleNormals	[triangleIndex + 2];

			glm::vec3 triNormal = normalize((modelVertexNormalA + modelVertexNormalB + modelVertexNormalC) / 3.0f);
			CheckTriangle(collisionPackage, modelVertexPositionA, modelVertexPositionB, modelVertexPositionC, triNormal, normal);
		}
	}
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

//----------------------------------------------------------------------------------

MotionHandler::PLANE::PLANE(const glm::vec3& origin, const glm::vec3& normal) {
	this->normal = normal;
	this->origin = origin;
	equation[0] = normal.x;
	equation[1] = normal.y;
	equation[2] = normal.z;
	equation[3] = -(normal.x * origin.x + normal.y * origin.y
		+ normal.z * origin.z);
}

// Construct from triangle:
MotionHandler::PLANE::PLANE(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& normal)
{
	this->normal = normal;
	origin = p1;
	equation[0] = normal.x;
	equation[1] = normal.y;
	equation[2] = normal.z;
	equation[3] = -(normal.x * origin.x + normal.y * origin.y
		+ normal.z * origin.z);
}

bool MotionHandler::PLANE::isFrontFacingTo(const glm::vec3& direction) const {
	double dot = glm::dot(normal, direction);
	return (dot <= 0);
}

float MotionHandler::PLANE::SignedDistanceTo(const glm::vec3& point) const {
	return glm::dot(point, normal) + equation[3];
}

typedef unsigned int uint32;
#define in(a) ((uint32&) a)

bool checkPointInTriangle(const glm::vec3& point,
	const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc)
{
	glm::vec3 e10 = pb - pa;
	glm::vec3 e20 = pc - pa;
	float a = glm::dot(e10, e10);
	float b = glm::dot(e10, e20);
	float c = glm::dot(e20, e20);
	float ac_bb = (a * c) - (b * b);
	glm::vec3 vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);
	float d = glm::dot(vp, e10);
	float e = glm::dot(vp, e20);
	float x = (d * c) - (e * b);
	float y = (e * a) - (d * b);
	float z = x + y - ac_bb;
	return ((in(z) & ~(in(x) | in(y))) & 0x80000000);
}

bool getLowestRoot(float a, float b, float c, float maxR,
	float* root) {
	// Check if a solution exists
	float determinant = b * b - 4.0f * a * c;
	// If determinant is negative it means no solutions.
	if (determinant < 0.0f) return false;
	// calculate the two roots: (if determinant == 0 then
	// x1==x2 but let’s disregard that slight optimization)
	float sqrtD = sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);
	// Sort so x1 <= x2
	if (r1 > r2) {
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}
	// Get lowest root:
	if (r1 > 0 && r1 < maxR) {
		*root = r1;
		return true;
	}
	// It is possible that we want x2 - this can happen
	// if x1 < 0
	if (r2 > 0 && r2 < maxR) {
		*root = r2;
		return true;
	}
	// No (valid) solutions
	return false;
}

float SquaredLength(glm::vec3 vector)
{
	float length = glm::length(vector);
	return length * length;
}

void MotionHandler::CheckTriangle(CollisionPacket* colPackage,
	const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& triNormal, glm::vec3& normal)
{
	// Make the plane containing this triangle.
	PLANE trianglePlane(p1, p2, p3, triNormal);
	// Is triangle front-facing to the velocity vector?
	// We only check front-facing triangles
	// (your choice of course)
	if (trianglePlane.isFrontFacingTo(
		colPackage->normalizedVelocity)) {
		// Get interval of plane intersection:
		float t0, t1;
		bool embeddedInPlane = false;
		// Calculate the signed distance from sphere
		// position to triangle plane
		float signedDistToTrianglePlane =
			trianglePlane.SignedDistanceTo(colPackage->basePoint);
		// cache this as we’re going to use it a few times below:
		float normalDotVelocity =
			glm::dot(trianglePlane.normal, colPackage->velocity);
		// if sphere is travelling parrallel to the plane:
		if (normalDotVelocity == 0.0f) {
			if (fabs(signedDistToTrianglePlane) >= 1.0f) {
				// Sphere is not embedded in plane.
				// No collision possible:
				return;
			}
			else {
				// sphere is embedded in plane.
				// It intersects in the whole range [0..1]
				embeddedInPlane = true;
				t0 = 0.0f;
				t1 = 1.0f;
			}
		}
		else {
			// N dot D is not 0. Calculate intersection interval:
			t0 = (-1.0f - signedDistToTrianglePlane) / normalDotVelocity;
			t1 = (1.0f - signedDistToTrianglePlane) / normalDotVelocity;
			// Swap so t0 < t1
			if (t0 > t1) {
				float temp = t1;
				t1 = t0;
				t0 = temp;
			}
			// Check that at least one result is within range:
			if (t0 > 1.0f || t1 < 0.0f) {
				// Both t values are outside values [0,1]
				// No collision possible:
				return;
			}
			// Clamp to [0,1]
			if (t0 < 0.0f) t0 = 0.0f;
			if (t1 < 0.0f) t1 = 0.0f;
			if (t0 > 1.0f) t0 = 1.0f;
			if (t1 > 1.0f) t1 = 1.0f;
		}
		// OK, at this point we have two time values t0 and t1
		// between which the swept sphere intersects with the
		// triangle plane. If any collision is to occur it must
		// happen within this interval.
		glm::vec3 collisionPoint;
		bool foundCollison = false;
		float t = 1.0;
		// First we check for the easy case - collision inside
		// the triangle. If this happens it must be at time t0
		// as this is when the sphere rests on the front side
		// of the triangle plane. Note, this can only happen if
		// the sphere is not embedded in the triangle plane.
		if (!embeddedInPlane) {
			glm::vec3 planeIntersectionPoint =
				(colPackage->basePoint - trianglePlane.normal)
				+ t0 * colPackage->velocity;
			if (checkPointInTriangle(planeIntersectionPoint,
				p1, p2, p3))
			{
				foundCollison = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}
		}
		// if we haven’t found a collision already we’ll have to
		// sweep sphere against points and edges of the triangle.
		// Note: A collision inside the triangle (the check above)
		// will always happen before a vertex or edge collision!
		// This is why we can skip the swept test if the above
		// gives a collision!
		if (foundCollison == false) {
			// some commonly used terms:
			glm::vec3 velocity = colPackage->velocity;
			glm::vec3 base = colPackage->basePoint;
			float velocitySquaredLength = SquaredLength(velocity);
			float a, b, c; // Params for equation
			float newT;
			// For each vertex or edge a quadratic equation have to
			// be solved. We parameterize this equation as
			// a*t^2 + b*t + c = 0 and below we calculate the
			// parameters a,b and c for each test.
			// Check against points:
			a = velocitySquaredLength;
			// P1
			b = 2.0f * glm::dot(velocity, base - p1);
			c = SquaredLength(p1 - base) - 1.0f;
			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				foundCollison = true;
				collisionPoint = p1;
			}
			// P2
			b = 2.0f * glm::dot(velocity, base - p2);
			c = SquaredLength(p2 - base) - 1.0f;
			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				foundCollison = true;
				collisionPoint = p2;
			}
			// P3
			b = 2.0f * glm::dot(velocity, base - p3);
			c = SquaredLength(p3 - base) - 1.0f;
			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				foundCollison = true;
				collisionPoint = p3;
			}
			// Check agains edges:
			// p1 -> p2:
			glm::vec3 edge = p2 - p1;
			glm::vec3 baseToVertex = p1 - base;
			float edgeSquaredLength = SquaredLength(edge);
			float edgeDotVelocity = glm::dot(edge, velocity);
			float edgeDotBaseToVertex = glm::dot(edge, baseToVertex);
			// Calculate parameters for equation
			a = edgeSquaredLength * -velocitySquaredLength +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * glm::dot(velocity, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1.0f - SquaredLength(baseToVertex)) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;
			// Does the swept sphere collide against infinite edge?
			if (getLowestRoot(a, b, c, t, &newT)) {
				// Check if intersection is within line segment:
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0f && f <= 1.0f) {
					// intersection took place within segment.
					t = newT;
					foundCollison = true;
					collisionPoint = p1 + f * edge;
				}
			}
			// p2 -> p3:
			edge = p3 - p2;
			baseToVertex = p2 - base;
			edgeSquaredLength = SquaredLength(edge);
			edgeDotVelocity = glm::dot(edge, velocity);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);
			a = edgeSquaredLength * -velocitySquaredLength +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * glm::dot(velocity, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - SquaredLength(baseToVertex)) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;
			if (getLowestRoot(a, b, c, t, &newT)) {
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					t = newT;
					foundCollison = true;
					collisionPoint = p2 + f * edge;
				}
			}
			// p3 -> p1:
			edge = p1 - p3;
			baseToVertex = p3 - base;
			edgeSquaredLength = SquaredLength(edge);
			edgeDotVelocity = glm::dot(edge, velocity);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);
			a = edgeSquaredLength * -velocitySquaredLength +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * glm::dot(velocity, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - SquaredLength(baseToVertex)) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;
			if (getLowestRoot(a, b, c, t, &newT)) {
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					t = newT;
					foundCollison = true;
					collisionPoint = p3 + f * edge;
				}
			}
		}
		// Set result:
		if (foundCollison == true) {
			// distance to collision: ’t’ is time of collision
			float distToCollision = t * colPackage->velocity.length();
			// Does this triangle qualify for the closest hit?
			// it does if it’s the first hit or the closest
			if (colPackage->foundCollision == false ||
				distToCollision < colPackage->nearestDistance) {
				normal = trianglePlane.normal;
				// Collision information nessesary for sliding
				colPackage->nearestDistance = distToCollision;
				colPackage->intersectionPoint = collisionPoint;
				colPackage->foundCollision = true;
			}
		}
	} // if not backface
}

// Set this to match application scale..
glm::vec3 MotionHandler::CollideWithWorld(const glm::vec3& pos, const glm::vec3& vel, glm::vec3& normal, int collisionRecursionDepth)
{
	CollisionPacket* collisionPackage = &packet;

	const float veryCloseDistance = 0.0001;

	// Give up on Resolution after checking too many times
	if (collisionRecursionDepth > maxRecursionDepth) 
		return pos;
	// Ok, we need to worry:
	collisionPackage->velocity = vel;

	collisionPackage->normalizedVelocity = normalize(vel);
	collisionPackage->basePoint = pos;
	collisionPackage->foundCollision = false;
	// Check for collision (calls the collision routines)
	// Application specific!!
	CheckCollision(collisionPackage, normal);
	// If no collision we just move along the velocity
	if (collisionPackage->foundCollision == false) {
		return pos + vel;
	}
	// *** Collision occured ***
	// The original destination point
	glm::vec3 destinationPoint = pos + vel;
	glm::vec3 newBasePoint = pos;
	// only update if we are not already very close
	// and if so we only move very close to intersection..not
	// to the exact spot.
	if (collisionPackage->nearestDistance >= veryCloseDistance)
	{
		glm::vec3 V = normalize(vel) * (collisionPackage->nearestDistance - veryCloseDistance);
		newBasePoint = collisionPackage->basePoint + V;
		// Adjust polygon intersection point (so sliding
		// plane will be unaffected by the fact that we
		// move slightly less than collision tells us)
		V = normalize(V);
		collisionPackage->intersectionPoint -=
			veryCloseDistance * V;
	}
	// Determine the sliding plane
	glm::vec3 slidePlaneOrigin = collisionPackage->intersectionPoint;
	glm::vec3 slidePlaneNormal = normalize(newBasePoint - collisionPackage->intersectionPoint);
	PLANE slidingPlane(slidePlaneOrigin, slidePlaneNormal);
	// Again, sorry about formatting.. but look carefully ;)
	glm::vec3 newDestinationPoint = destinationPoint -
		slidingPlane.SignedDistanceTo(destinationPoint) *
		slidePlaneNormal;
	// Generate the slide vector, which will become our new
	// velocity vector for the next iteration
	glm::vec3 newVelocityVector = newDestinationPoint -
		collisionPackage->intersectionPoint;
	// Recurse:
	// dont recurse if the new velocity is very small
	if (newVelocityVector.length() < veryCloseDistance) {
		return newBasePoint;
	}

	return CollideWithWorld(newBasePoint, newVelocityVector, normal, ++collisionRecursionDepth);
}

glm::vec3 MotionHandler::CollideAndSlide(const glm::vec3& position, const glm::vec3& vel, glm::vec3& normal)
{
	glm::vec3 gravity(0.0f, -5.0f, 0.0f);

	normal = glm::vec3(0.0f, 0.0f, 0.0f);
	CollisionPacket* collisionPackage = &packet;
	// Do collision detection:
	collisionPackage->R3Position = position;
	collisionPackage->R3Velocity = vel;

	// calculate position and velocity in eSpace
	glm::vec3 eSpacePosition = collisionPackage->R3Position /
		collisionPackage->eRadius;
	glm::vec3 eSpaceVelocity = collisionPackage->R3Velocity /
		collisionPackage->eRadius;

	// Iterate until we have our final position.
	glm::vec3 finalPosition = CollideWithWorld(eSpacePosition,
		eSpaceVelocity, normal);

	// Add gravity pull:
	/*
	// To remove gravity comment from here .....
	if (normal == glm::vec3(0.0f, 0.0f, 0.0f))
	{
		collisionPackage->R3Position =
			finalPosition * collisionPackage->eRadius;
		collisionPackage->R3Velocity = gravity;
		eSpaceVelocity = gravity / collisionPackage->eRadius;
		finalPosition = CollideWithWorld(finalPosition,
			eSpaceVelocity, normal);
	}
	// ... to here
	*/
	// Convert final result back to R3:
	finalPosition = finalPosition * collisionPackage->eRadius;
	// Move the entity (application specific function)
	return finalPosition;
}
