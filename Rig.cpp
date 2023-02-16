#include "Rig.h"

Rig::Rig
(
	glm::vec3 initTranslation,
	glm::quat initRotation, 
	glm::vec3 initScale
):
	translation(initTranslation),
	rotation(initRotation),
	scale(initScale)
{}

void Rig::AddModel(RiggedModel model)
{
	riggedModels.push_back(model);
}

//TODO - mesh warp?
void Rig::Draw(Shader& shader)
{
	for (int i = 0; i < riggedModels.size(); ++i)
	{
		Model& riggedModel = riggedModels[i].model;

		// convert transformations to matricies
		glm::mat4 translationMat = glm::mat4(1.0f);
		glm::mat4 rotationMat = glm::mat4(1.0f);
		glm::mat4 scaleMat = glm::mat4(1.0f);

		translationMat = glm::translate(translationMat, riggedModels[i].translation);
		rotationMat = glm::mat4_cast(riggedModels[i].rotation);
		scaleMat = glm::scale(scaleMat, riggedModels[i].scale);

		// Create updated Transformation matrix
		glm::mat4 transformMatrix = glm::mat4(1.0f) * translationMat * rotationMat * scaleMat;

		riggedModel.Draw(shader, translation, rotation, scale, transformMatrix);
	}
}