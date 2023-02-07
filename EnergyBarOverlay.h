
#pragma once
#include "Overlay.h"
#include "Player.h"

class EnergyBarOverlay :
    public Overlay
{
public:
    EnergyBarOverlay(Player& player);
    void Update();
private:
    Player& player;
    const float xMax = 0.0f;
    const float xMin = -0.72f;
    const float yMax = 0.39f;
    const float yMin = 0.38f;
    const float borderWidth = 0.001f;
};

