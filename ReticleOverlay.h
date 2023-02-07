#pragma once
#include "Overlay.h"
#include "Player.h"

class ReticleOverlay :
    public Overlay
{
public:
    ReticleOverlay(Player& player);
    void Update();
private:
    void UpdateVertices();
    float zoomFac;
    const float zoomIncrement = 1.0f;
    const float maxRadius = 0.10;
    const float minRadius = 0.05;
    const float rotationOffset = 0.05;
    const float radialOffset = 0.018;
    Player& player;

};
