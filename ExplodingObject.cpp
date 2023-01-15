#include "ExplodingObject.h"

void ExplodingObject::Update(double new_phase)
{
	phase = (float)new_phase;
}

void ExplodingObject::Draw(Shader& shader)
{
	shader.Activate();
	glUniform1fv(glGetUniformLocation(shader.ID, "phase"), 1, &phase);
	glUniform1fv(glGetUniformLocation(shader.ID, "amplitude"), 1, &amplitude);
	glUniform1fv(glGetUniformLocation(shader.ID, "speed"), 1, &speed);
	glUniform1i(glGetUniformLocation(shader.ID, "explode"), true);
	model.Draw(shader, translation, rotation, scale);
}
