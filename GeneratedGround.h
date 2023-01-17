#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "GameObject.h"

class GeneratedGround :
	public GameObject
{
public:
	GeneratedGround(
		Camera* camera,
		const char* modelFile,
		glm::vec3 initTranslation = glm::vec3(0),
		glm::quat initRotation = glm::quat(0.0, 0.0, 0.0, 0.0),
		glm::vec3 initScale = glm::vec3(0));

	void Draw(Shader& shader);

	void UpdatePanels();

	void IntializePanelDimensions();

	void UpdateWorldVertices();
private:
	glm::vec3 GetPanelScale(int panelIndex);
	glm::vec3 GetPanelTranslation(int panelIndex);

	Camera* camera;
	const int panelsToEdge = 1;
	const float buffer = 8.0f;
	float panelWidth;
	float panelLength;
	int flipOffsetX;
	int flipOffsetZ;

};