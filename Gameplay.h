#pragma once
#include "Scene.h"
#include "Camera.h"
#include "NetworkHandler.h"
class GamePlay :
    public Scene
{
public:
    GamePlay(float width, float height);
    Scene* Update(double time);
private:
    Camera camera;
};

