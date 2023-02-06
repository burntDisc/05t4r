#include "ReticleOverlay.h"

#include "EnergyBarOverlay.h"

// TODO: MAKE IT WORK!!
// TEMPLATED ON ENERGY BAR
ReticleOverlay::ReticleOverlay(Player& player) :
	player(player),
	Overlay::Overlay()
{
	std::vector<Vertex2D> barVertices;

	// right corners of bar
	barVertices.push_back(Vertex2D(glm::vec2(0.5f, 0.95f), glm::vec3(0.5f, 0.0f, 1.0f)));
	barVertices.push_back(Vertex2D(glm::vec2(0.5f, 0.92f), glm::vec3(0.5f, 0.0f, 1.0f)));

	std::vector<GLuint> barIndices;

	// first bar triangle
	barIndices.push_back(0);
	barIndices.push_back(1);
	barIndices.push_back(2);

	vertices = barVertices;
	indices = barIndices;
	SetVertices();
}

void ReticleOverlay::Update()
{
	float rightCornerX = player.energy - 0.5f;
	vertices[0].position[0] = rightCornerX;
	vertices[1].position[0] = rightCornerX;
	GLint offset = 0;
	VAO.Bind();

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2), (void*)&vertices[0].position);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex2D), sizeof(glm::vec2), (void*)&vertices[1].position);
}
