#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Shader.h"
#include "Model.h"
#include "Player.h"
#include "GameObject.h"

class GeneratedGround :
	public GameObject,
	public Updatable,
	public Drawable
{
public:
	GeneratedGround(
		Shader shader,
		Player& player,
		const char* modelFile,
		glm::vec3 initTranslation = glm::vec3(0),
		glm::vec3 initScale = glm::vec3(0),
		glm::quat initRotation = glm::quat(0.0, 0.0, 0.0, 0.0));

	void Draw();

	void Update(double time);

	void IntializePanelDimensions();

	void UpdateWorldVertices();
private:
	glm::vec3 GetPanelScale(int panelIndex, int numToEdge);
	glm::vec3 GetPanelTranslation(int panelIndex, int numToEdge);

	Player& player;

	// pushes non-rectangular models together by buffer 
	const float buffer = 8.0f;

	int visiblePanelsToEdge = 13; // must be odd TODO: handle even
	const int solidPanelsToEdge = 1;

	float panelWidth;
	float panelLength;
	int flipOffsetX;
	int flipOffsetZ;

};