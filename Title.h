#pragma once
#include "Scene.h"
#include "Text.h"
#include "IPSelector.h"

class Title:
    public Scene
{
public:
    Title();
    void Draw();
private:
    Shader shader2D = Shader("shaders/2D.vert", "shaders/2D.frag");
    IPSelector ipSelector;
};

