#pragma once
#include "Overlay.h"
#include "Updatable.h"
#include "Player.h"


class HealthBarOverlay :
    public Overlay,
    public Updatable,
    public Deletable
{
public:
    HealthBarOverlay(Shader shader, Player& player);
    void Update(double time);
    void Delete();
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
    const glm::vec3 color0 = glm::vec3(0.5f, 0.0f, 1.0f);
    const glm::vec3 color1 = glm::vec3(1.0f, 0.7f, 0.7f);
    const glm::vec3 color2 = glm::vec3(0.7f, 1.0f, 0.7f);
};

