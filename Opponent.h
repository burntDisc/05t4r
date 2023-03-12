#pragma once

#include "Rig.h"
#include "ProjectileStream.h"
#include "NetworkHandler.h"
#include "Updatable.h"


// TODO: for dummy update.. remove
class Player;

class Opponent :
	public Rig,
	public Updatable
{
public:
	Opponent(
		Shader shader,
		const char* modelFile,
		ProjectileStream& projectileStream,
		glm::vec3 initTranslation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 initScale = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat initRotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 modelOffset = glm::vec3(0.0f, 0.0f, 0.0f));

	void Update(double time);
	void DummyUpdate(double time, Player& player);
	glm::vec3 GetPosition();
private:
	enum PartIndex { chest, head, leftArm, rightArm, leftLeg, rightLeg };
	int collisions;
	float phase = 0.0f;
	float prevDistance, prevPrevDistance;
	float walkMagnitude = 0.0f;
	void UpdateRig(float travel, float speed, bool colliding);
	NetworkHandler::Gamestate state = {
		.translation = glm::vec3(10.0f,10.0f,10.0f),
		.orientation = glm::vec3(0.0f,0.0f,1.0f),
		.time = 0.0,
		.firingIntensity = 0.0f,
		.firing = false,
		.valid = true
	};;
	double prevStateTime;
	glm::quat LookRotation(glm::vec3 orientation);
	glm::quat modelRotation;
	ProjectileStream& projectileStream;
};

