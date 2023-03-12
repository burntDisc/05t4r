#pragma once
#include "Overlay.h"
#include "Text.h"
#include <string>

class IPSelector :
    public Overlay
{
public:
    IPSelector(Shader shader, glm::vec2 origin, float size);
    void Draw();
    void Update(double time);
private:
    Text text;
    std::string ip;
};

