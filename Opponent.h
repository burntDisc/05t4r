#pragma once

#include "Rig.h"
#include "ProjectileStream.h"
#include "NetworkHandler.h"

class Opponent :
	public Rig
{
public:
	Opponent(
		const char* modelFile,
		ProjectileStream& projectileStream,
		glm::vec3 initTranslation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 initScale = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat initRotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 modelOffset = glm::vec3(0.0f, 0.0f, 0.0f));

	void Update(double time);
	void DummyUpdate(double time);
	glm::vec3 GetPosition();
private:
	enum PartIndex { chest, head, leftArm, rightArm, leftLeg, rightLeg };
	void UpdateLeftLeg(double time);
	void UpdateRightLeg(double time);
	NetworkHandler::Gamestate state;
	double prevStateTime;
	double loopTime;
	glm::quat LookRotation(glm::vec3 orientation);
	glm::quat modelRotation;
	ProjectileStream& projectileStream;
};

