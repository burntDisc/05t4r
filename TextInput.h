#pragma once
#include "Overlay.h"
#include "Updatable.h"
#include "Deletable.h"
#include "Text.h"
#include <string>

class TextInput :
    public Overlay,
    public Updatable,
    public Deletable
{
public:
    TextInput(Shader shader, glm::vec2 origin, float size, int numChars);
    ~TextInput();
    void Draw();
    void Update(double time);
    void Delete();

    std::string GetText();
    bool selected = false;
protected:
    int selectedIndex = 0;
    float activeChar = 0.0f;
    float characterOffset = 0.0f;
    Text* text;
    virtual void UpdateGraphics(int index = -1);
    virtual void ShiftDigit(int index);
private:
    float size;
    glm::vec2 origin;

    float charSelectSpeed = 0.0f;
    const float charSelectAccel = 14.0f;

    float charShiftSpeed = 0.0f;
    const float charShiftAccel = 14.0f;

    const glm::vec3 selectorColor = glm::vec3(0.5f, 0.5f, 1.0f);
};