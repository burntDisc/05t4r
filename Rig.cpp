#include "Rig.h"

Rig::Rig(int numModels, ...)
{
	va_list valist;
	va_start(valist, numModels);

	for (int i = 0; i < numModels; i++) { 
		riggedModels.push_back(va_arg(valist, RiggedModel));
	}

	va_end(valist); 

}

void Rig::Draw(Shader& shader, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	for (int i = 0; i < riggedModels.size(); ++i)
	{
		Model& riggedModel = riggedModels[i].model;

		// convert transformations to matricies
		glm::mat4 translationMat = glm::mat4(1.0f);
		glm::mat4 rotationMat = glm::mat4(1.0f);
		glm::mat4 scaleMat = glm::mat4(1.0f);

		translationMat = glm::translate(translationMat, riggedModels[i].translationOffset);
		//rotationMat = glm::mat4_cast(riggedModels[i].rotationOffset);
		//scaleMat = glm::scale(scaleMat, scale);

		// Create updated Transformation matrix
		glm::mat4 transformMatrix = glm::mat4(1.0f) * translationMat * rotationMat * scaleMat;

		riggedModel.Draw(shader, translation, rotation, scale, transformMatrix);
	}
}