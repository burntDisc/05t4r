#include "HealthBarOverlay.h"

//TODO: MAKE IT WORK USING TEMPLATE BELOW
HealthBarOverlay::HealthBarOverlay(Player& player) :
	player(player),
	Overlay::Overlay()
{
	std::vector<Vertex2D> barsVertices;

	// create bars
	for (int i = 0; i < 64; ++i)
	{
		vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.95f), glm::vec3(0.5f, 0.0f, 1.0f)));
		vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.92f), glm::vec3(0.5f, 0.0f, 1.0f)));
		vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.95f), glm::vec3(0.5f, 0.0f, 1.0f)));
		vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.92f), glm::vec3(0.5f, 0.0f, 1.0f)));

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);
	}

	//create background
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.95f), glm::vec3(0.5f, 0.0f, 1.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.92f), glm::vec3(0.5f, 0.0f, 1.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.95f), glm::vec3(0.5f, 0.0f, 1.0f)));
	vertices.push_back(Vertex2D(glm::vec2(0.5f, 0.92f), glm::vec3(0.5f, 0.0f, 1.0f)));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);
	SetVertices();
}

void HealthBarOverlay::Update()
{
	float rightCornerX = player.energy - 0.5f;
	vertices[0].position[0] = rightCornerX;
	vertices[1].position[0] = rightCornerX;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2), (void*)&vertices[0].position);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex2D), sizeof(glm::vec2), (void*)&vertices[1].position);
}