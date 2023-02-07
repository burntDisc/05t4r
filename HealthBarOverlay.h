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
    Player& player;
    const float xMax = 0.5f;
    const float xMin = -0.5f;
    const float yMax = 0.4f;
    const float yMin = 0.38f;
    const float borderWidth = 0.001f;
};

