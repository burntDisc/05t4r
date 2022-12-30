#include "GameObject.h"

GameObject::GameObject(const char* modelFile, glm::vec3 initTranslation, glm::vec3 initRotation, glm::vec3 initScale) :
	model(modelFile), translation(initTranslation), rotation(initRotation), scale(initScale)
{
}

void GameObject::Draw(Shader& shader)
{
	glDepthFunc(GL_LESS);
	model.Draw(shader, translation, rotation, scale);
}

void GameObject::Update(float time)
{
}

std::vector<Mesh>& GameObject::getMeshes()
{
	return model.getMeshes();
}
