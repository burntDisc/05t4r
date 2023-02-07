#pragma once
#include "Overlay.h"
#include "Player.h"


class HealthBarOverlay :
    public Overlay
{
public:
    HealthBarOverlay(Player& player);
    void Update();
private:
    void UpdateIndices(int bars);
    Player& player;
    const int numBars = 64;
    const float barBuffer = 0.002f;
    const float borderWidth = barBuffer;
    const float xMax = -0.65f;
    const float xMin = -0.72f;
    const float yMax = 0.4f;
    const float yMin = -0.4f;
};

