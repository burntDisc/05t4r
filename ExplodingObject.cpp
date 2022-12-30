#include "ExplodingObject.h"

void ExplodingObject::Step(float currentTime)
{
	time = currentTime;
}

void ExplodingObject::Draw(Shader& shader)
{
	shader.Activate();
	glUniform1fv(glGetUniformLocation(shader.ID, "time"), 1, &time);
	glUniform1i(glGetUniformLocation(shader.ID, "explode"), true);
	model.Draw(shader, translation, rotation, scale);
}
