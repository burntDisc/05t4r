
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
    const float xMax = 0.80f;
    const float xMin = -0.80f;
    const float yMax = 0.99f;
    const float yMin = 0.91f;
    const float borderWidth = 0.01f;
};

