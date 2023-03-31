#pragma once
#include "Overlay.h"
#include "Deletable.h"
#include "Updatable.h"
#include "Char.h"
#include <string>

class Text :
    public Overlay,
    public Deletable,
    public Updatable
{
public:
    Text(Shader shader, std::string rawText, glm::vec2 origin, float size);
    size_t size();
    void Draw();
    void Update(double time);
    void ChangeChar(int index, char value);
    void ChangeText(std::string newText);
    char GetChar(int index);
    std::string GetText();
    void Delete();
private:
    bool visible = true;
    double lastBlink = 0;
    const double blinkInterval = 0.5;
    std::vector<Char> chars;
};


