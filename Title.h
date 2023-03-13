#pragma once
#include "Scene.h"
#include "Text.h"
#include "IPSelector.h"
#include "GamePlay.h"

class Title:
    public Scene
{
public:
    Title(int width, int height);
    Scene* Update(double time);
    void Draw();
private:
    IPSelector* ipSelector = nullptr;
    GamePlay* preLoadedGame = nullptr;
    Shader shader2D = Shader("shaders/2D.vert", "shaders/HUD.frag");
};

