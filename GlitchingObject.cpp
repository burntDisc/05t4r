#include "GlitchingObject.h"
#include <numbers>
#include <iostream>

void GlitchingObject::Update(double time)
{
	//2 * acos(0.0) is pi
	float cycleTime = 2 * acos(0.0)/speed;
	factor = ((sin(time * speed - cycleTime/2))) * amplitude;
	if (time > prevCycle + cycleTime)
	{
		std::cout << "newLevel: ";
		prevCycle = time;
		level = (float)(rand() % 20 - 5);
		std::cout << level << std::endl;
	}
}

void GlitchingObject::Draw(Shader& shader)
{
	shader.Activate();
	glUniform1fv(glGetUniformLocation(shader.ID, "factor"), 1, &factor);
	glUniform1fv(glGetUniformLocation(shader.ID, "level"), 1, &level);
	model.Draw(shader, translation, rotation, scale);
}
