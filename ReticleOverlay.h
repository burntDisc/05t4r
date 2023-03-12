#pragma once
#include "Overlay.h"
#include "Player.h"

class ReticleOverlay :
    public Overlay
{
public:
    ReticleOverlay(Shader shader, Player& player, int windowWidth, int windowHeight);
    void Update(double time);
private:
    float prevTime = 0.0f;
    float windowWidth, windowHeight;
    void UpdateVertices(float delta);
    float zoomFac;
    const float zoomSpeed = 0.1f;
    const float maxRadius = 0.30;
    const float minRadius = 0.03;
    const float rotationOffset = 0.05;
    const float radialOffset = 0.05;

    const glm::vec3 color = glm::vec3(1.0f, 0.7f, 0.7f);
    const glm::vec3 accentColor = glm::vec3(0.0f, 1.0f, 0.0f);

    Player& player;

};
