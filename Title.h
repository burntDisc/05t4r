#pragma once
#include "Text.h"
#include "IPSelector.h"

class Title
{
public:
    Title();
    void Draw(Shader shader);
private:
    IPSelector ipSelector;
};

