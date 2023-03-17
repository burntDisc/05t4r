#pragma once
#include "TextInput.h"

class IPSelector :
    public TextInput
{
public:
    IPSelector(Shader shader, glm::vec2 origin, float size);
    int GetStringOffset(int index);
    void ShiftDigit(int index);
    void UpdateGraphics(int index);
    std::string GetIP();
};

