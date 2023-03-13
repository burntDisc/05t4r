
#pragma once
#include "Overlay.h"
#include "Player.h"

class EnergyBarOverlay :
    public Overlay,
    public Updatable,
    public Deletable
{
public:
    EnergyBarOverlay(Shader shader, Player& player);
    void Update(double time);
    void Delete();
private:
    Player& player;
    const float xMax = 0.60f;
    const float xMin = -0.60f;
    const float yMax = 0.93f;
    const float yMin = 0.90f;
    const float borderWidth = 0.005f;
};

