#pragma once
#include "Overlay.h"
#include "Player.h"


class HealthBarOverlay :
    public Overlay
{
public:
    HealthBarOverlay(Shader shader, Player& player);
    void Update(double time);
private:
    void UpdateIndices(int bars);
    Player& player;
    const int numBars = 64;
    const float barBuffer = 0.01f;
    const float borderWidth = barBuffer;
    const float xMax = -0.85f;
    const float xMin = -0.96f;
    const float yMax = 0.96f;
    const float yMin = -0.96f;
};

