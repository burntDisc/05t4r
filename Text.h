#pragma once
#include "Overlay.h"
#include "Deletable.h"
#include "Char.h"
#include <string>

class Text :
    public Overlay,
    public Deletable
{
public:
    Text(Shader shader, std::string rawText, glm::vec2 origin, float size);
    void Draw();
    void ChangeChar(int index, char value);
    void Delete();
private:
    std::vector<Char> chars;
};

