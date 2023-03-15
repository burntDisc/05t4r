#pragma once
#include "Overlay.h"
#include "Updatable.h"
#include "Deletable.h"
#include "Text.h"
#include <string>

class IPSelector :
    public Overlay,
    public Updatable,
    public Deletable
{
public:
    IPSelector(Shader shader, glm::vec2 origin, float size);
    void Draw();
    void Update(double time);
    void Delete();

    std::string GetIP();
    bool selected = false;
    std::string ip;
private:
    float size;
    glm::vec2 origin;

    void ShiftDigit(int activeDigit);
    void UpdateSelector();
    int GetStringOffset(int index);

    float activeDigit = 0.0f;
    float digitSelectSpeed = 0.0f;
    const float digitSelectAccel = 14.0f;

    float characterOffset = 0.0f;
    float digitShiftSpeed = 0.0f;
    const float digitShiftAccel = 14.0f;

    const glm::vec3 selectorColor = glm::vec3(0.5f, 0.5f, 1.0f);
    Text text;
};

