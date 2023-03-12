#pragma once
#include "Overlay.h"
#include "Char.h"
#include <string>
class Text :
    public Overlay
{
public:
    Text(Shader shader, std::string rawText, glm::vec2 origin, float size);
    void Draw();
private:
    std::vector<Char> chars;
};

