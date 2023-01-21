#include "GlitchingObject.h"
#include <iostream>

void GlitchingObject::Update(float time)
{
	//2 * acos(0.0) is pi
	float cycleTime = 2 * (float)acos(0.0) / speed;
	factor = (float)sin(time * speed - cycleTime/2) * amplitude;
	if (time > prevCycle + cycleTime)
	{
		prevCycle = time;
		level = (float)(rand() % 20 - 5);
	}
}

void GlitchingObject::Draw(Shader& shader)
{
	shader.Activate();
	glUniform1fv(glGetUniformLocation(shader.ID, "factor"), 1, &factor);
	glUniform1fv(glGetUniformLocation(shader.ID, "level"), 1, &level);
	model.Draw(shader, translation, rotation, scale);
}
