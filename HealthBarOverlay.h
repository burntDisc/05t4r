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
};

