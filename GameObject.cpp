#include "GameObject.h"

GameObject::GameObject(const char* modelFile, glm::vec3 initTranslation, glm::vec3 initRotation, glm::vec3 initScale) :
	model(modelFile), translation(initTranslation), rotation(initRotation), scale(initScale)
{
}

void GameObject::Draw(Shader& shader)
{
	model.Draw(shader, translation, rotation, scale);
}

void GameObject::Update(float time)
{
}
