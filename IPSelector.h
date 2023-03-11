#pragma once
#include "Overlay.h"
#include "Text.h"
#include <string>

class IPSelector :
    public Overlay
{
public:
    IPSelector(glm::vec2 origin, float size);
    void Draw(Shader shader);
    void Update(double time);
private:
    Text text;
    std::string ip;
};

