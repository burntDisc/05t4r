#include "Opponent.h"

#include <iostream>
#include <cmath>

Opponent::Opponent
(
	const char* modelFile,
    ProjectileStream& projectileStream,
	glm::vec3 initTranslation,
	glm::vec3 initScale,
	glm::quat initRotation,
    glm::vec3 modelOffset
) :
    projectileStream(projectileStream),
	modelRotation(initRotation),
	GameObject(
		modelFile,
		initTranslation,
		initScale,
		initRotation,
        modelOffset)
{}

// define z+ as forward

glm::quat Opponent::LookRotation(glm::vec3 orientation)
{
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    glm::vec3 interim0 = glm::cross(up, orientation);

    glm::vec3 interim1 = glm::cross(orientation, interim0);
    float m00 = interim0.x;
    float m01 = interim0.y;
    float m02 = interim0.z;
    float m10 = interim1.x;
    float m11 = interim1.y;
    float m12 = interim1.z;
    float m20 = orientation.x;
    float m21 = orientation.y;
    float m22 = orientation.z;

    double num8 = (m00 + m11) + m22;
    glm::quat quaternion;
    if (num8 > 0.0)
    {
        float num = (float)sqrt(num8 + 1.0);
        quaternion.w = (num * 0.5f);
        num = 0.5f / num;
        quaternion.x = (m12 - m21) * num;
        quaternion.y = (m20 - m02) * num;
        quaternion.z = (m01 - m10) * num;
        return quaternion;
    }
    if ((m00 >= m11) && (m00 >= m22))
    {
        float num7 = (float)sqrt(((1.0 + m00) - m11) - m22);
        float num4 = 0.5f / num7;
        quaternion.x = 0.5f * num7;
        quaternion.y = (m01 + m10) * num4;
        quaternion.z = (m02 + m20) * num4;
        quaternion.w = (m12 - m21) * num4;
        return quaternion;
    }
    if (m11 > m22)
    {
        float num6 = (float)sqrt(((1.0 + m11) - m00) - m22);
        float num3 = 0.5f / num6;
        quaternion.x = (m10 + m01) * num3;
        quaternion.y = 0.5f * num6;
        quaternion.z = (m21 + m12) * num3;
        quaternion.w = (m20 - m02) * num3;
        return quaternion;
    }
    float num5 = (float)sqrt(((1.0 + m22) - m00) - m11);
    float num2 = 0.5f / num5;
    quaternion.x = (m20 + m02) * num2;
    quaternion.y = (m21 + m12) * num2;
    quaternion.z = 0.5f * num5;
    quaternion.w = (m01 - m10) * num2;
    return quaternion;
}

void Opponent::Update(double time)
{
    loopTime = time - prevStateTime;
    prevStateTime = time;

    glm::vec3 rotationAdjustment(0.0f, 0.0f, 0.0f);

	state = NetworkHandler::GetRemoteGamestate(state.time + loopTime, state);

    rotation = LookRotation(state.orientation) * modelRotation;
    translation = state.translation;

    if (state.firing)
    {
        projectileStream.Fire(translation, state.orientation, &state.firingIntensity);
    }
}

glm::vec3 Opponent::GetPosition()
{
    return translation;
}

