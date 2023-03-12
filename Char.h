#pragma once
#include "Overlay.h"
class Char :
    public Overlay
{
public:
    Char(Shader shader, char character, glm::vec2 origin, float size);
private:
    void Init_dot();
    void Init_0();
    void Init_1();
    void Init_2();
    void Init_3();
    void Init_4();
    void Init_5();
    void Init_6();
    void Init_7();
    void Init_8();
    void Init_9();

    void Init_r();
    void Init_t();
};

