#pragma once
#include "Scene.h"
#include "Camera.h"
#include "NetworkHandler.h"
class Gameplay :
    public Scene
{
public:
    Gameplay(float with, float height);
    Scene* Update(double time);
private:
    NetworkHandler networkHandler;
    Camera camera;
};

