
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
    const float xMax = 0.60f;
    const float xMin = -0.60f;
    const float yMax = 0.93f;
    const float yMin = 0.90f;
    const float borderWidth = 0.005f;
};

