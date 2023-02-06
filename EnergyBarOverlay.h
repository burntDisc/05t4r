
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
};

