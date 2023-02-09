
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
    const float xMax = 0.70f;
    const float xMin = -0.70f;
    const float yMax = 0.94f;
    const float yMin = 0.90f;
    const float borderWidth = 0.008f;
};

