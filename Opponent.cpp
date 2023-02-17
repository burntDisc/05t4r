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
    Rig(initTranslation,
        initRotation,
        initScale),
    projectileStream(projectileStream),
    modelRotation(initRotation)
{
    glm::quat tempRot = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 legRotationOffset = glm::vec3(0.0f, 0.0f, acos(0.0f) / 3); 
    glm::vec3 armRotationOffset = glm::vec3(0.0f, 0.0f, acos(0.0f) / 3);

    //chest
    Rig::AddModel(Rig::RiggedModel{ Model(modelFile), glm::vec3(0.0f,0.0f,0.0f), tempRot, glm::vec3(0.7f, 1.0f, 0.7f) });
    //head
    Rig::AddModel(Rig::RiggedModel{ Model(modelFile), glm::vec3(0.0f,1.0f,0.0f), tempRot, glm::vec3(0.5f, 0.5f, 0.5f) });
    //arms
    Rig::AddModel(Rig::RiggedModel{ Model(modelFile), glm::vec3(1.0f, -0.1f, 0.0f), armRotationOffset, glm::vec3(0.3f, 1.0f, 0.3f) });
    Rig::AddModel(Rig::RiggedModel{ Model(modelFile), glm::vec3(-1.0f, -0.1f, 0.0f), -armRotationOffset, glm::vec3(0.3f, 1.0f, 0.3f) });
    //legs
    Rig::AddModel(Rig::RiggedModel{ Model(modelFile), glm::vec3(0.5f,-1.5f, 0.0f),legRotationOffset, glm::vec3(0.3f, 1.0f, 0.3f) });
    Rig::AddModel(Rig::RiggedModel{ Model(modelFile), glm::vec3(-0.5f,-1.5f, 0.0f), -legRotationOffset, glm::vec3(0.3f, 1.0f, 0.3f) });
}

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

//TODO handle interloop-firing?

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
    glm::vec3 xzVelocity = state.velocity - glm::vec3(0.0f, state.velocity.z, 0.0f);
    UpdateRig(translation.x + translation.y, glm::length(xzVelocity));
}



glm::vec3 Opponent::GetPosition()
{
    return translation;
}


void Opponent::UpdateRig(double positionFac, float speed)
{
    float zOffsetAngle = 0.02f;
    float speedFac = 1.0f;
    float legHeight = -1.5f;
    float armHeight = 1.0f;
    float legLength = 2.0f;
    float armLength = 2.0f;
    walkMagnitude =
        state.colliding?
        speed / 2.0f :
        std::max(0.0f, walkMagnitude - walkdecay); 
    float phaseA = positionFac * speedFac;

    riggedModels[leftLeg].translation.z = walkMagnitude * sin(phaseA);
    riggedModels[leftLeg].translation.y = legHeight  - walkMagnitude * abs(legLength * cos(phaseA));
    riggedModels[leftLeg].rotation = glm::quat(glm::vec3(walkMagnitude * sin(-phaseA), 0.0f, zOffsetAngle));

    riggedModels[rightArm].translation.z = walkMagnitude * sin(phaseA);
    riggedModels[rightArm].translation.y = armHeight  - walkMagnitude * abs(armLength * cos(phaseA));
    riggedModels[rightArm].rotation = glm::quat(glm::vec3(walkMagnitude * sin(-phaseA), 0.0f, -zOffsetAngle));

    float phaseB = positionFac * speedFac + 2 * acos(0);

    riggedModels[rightLeg].translation.z = walkMagnitude * sin(phaseB);
    riggedModels[rightLeg].translation.y = legHeight  - walkMagnitude * abs(legLength * cos(phaseB));
    riggedModels[rightLeg].rotation = glm::quat(glm::vec3(walkMagnitude * sin(-phaseB), 0.0f, -zOffsetAngle));

    riggedModels[leftArm].translation.z = walkMagnitude * sin(phaseB);
    riggedModels[leftArm].translation.y = armHeight  - walkMagnitude * abs(armLength * cos(phaseB));
    riggedModels[leftArm].rotation = glm::quat(glm::vec3(walkMagnitude * sin(-phaseB), 0.0f, zOffsetAngle));
}
