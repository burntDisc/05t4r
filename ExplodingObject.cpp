#include "ExplodingObject.h"

ExplodingObject::ExplodingObject(const char* modelFile, glm::vec3 initTranslation, glm::vec3 initScale, glm::quat initRotation, glm::vec3 modelOffset):
	GameObject(
		modelFile,
		initTranslation,
		initScale,
		initRotation, 
		modelOffset) 
{
}

void ExplodingObject::Update(double time)
{
	phase = (float)time;
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
