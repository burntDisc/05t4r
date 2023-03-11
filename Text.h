#pragma once
#include "Overlay.h"
#include "Char.h"
#include <string>
class Text :
    public Overlay
{
public:
    Text(std::string rawText, glm::vec2 origin, float size);
    void Draw(Shader shader);
private:
    std::vector<Char> chars;
};

