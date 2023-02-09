#pragma once
#include "Overlay.h"
#include "Player.h"

class ReticleOverlay :
    public Overlay
{
public:
    ReticleOverlay(Player& player, int windowWidth, int windowHeight);
    void Update();
private:
    float windowWidth, windowHeight;
    void UpdateVertices();
    float zoomFac;
    const float zoomIncrement = 1.0f;
    const float maxRadius = 0.30;
    const float minRadius = 0.03;
    const float rotationOffset = 0.1;
    const float radialOffset = 0.05;
    Player& player;

};
