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
    const float xMax = -0.7f;
    const float xMin = -0.8f;
    const float yMax = 0.8f;
    const float yMin = -0.8f;
};

