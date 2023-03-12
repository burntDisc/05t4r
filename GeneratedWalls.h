#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "Model.h"
#include "Player.h"
#include "Drawable.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class GeneratedWalls :
    public GameObject,
	public Updatable,
	public Drawable
{
public:
	//TODO use player to dynamically generate colliion objects like floor
	GeneratedWalls(
		Shader shader,
		Player& player,
		const char* modelFile,
		glm::vec3 initTranslation,
		glm::vec3 initScale,
		glm::quat initRotation);

	void Draw();

	void Update(double time);
private:
	const glm::vec3 clusterDimensions = glm::vec3(10.0f, 10.0f, 10.0f);

	const int solidClustersToEdge = 1;
	const int visibleClustersToEdge = 1;
	const int wallsPerCluster = 8;


	void SetWallDimensions();

	glm::vec3 GetClusterTranslation(int clusterIndex, int numToEdge);

	glm::vec3 GetWallTranslation(int wallIndex, int clusterIndex);

	void UpdateWorldVertices();

	glm::vec3 wallDimensions;

	Player& player;

};
