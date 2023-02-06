#pragma once
#include "Overlay.h"
#include "Player.h"

class ReticleOverlay :
    public Overlay
{
public:
    ReticleOverlay(Player& player);
    void Update();
private:
    Player& player;

};
