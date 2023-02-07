#include "HealthBarOverlay.h"

//TODO: MAKE IT WORK USING TEMPLATE BELOW
HealthBarOverlay::HealthBarOverlay(Player& player) :
	player(player),
	Overlay::Overlay()
{
	std::vector<Vertex2D> barVertices;

	// right corners of bar
	barVertices.push_back(Vertex2D(glm::vec2(xMax, yMax), glm::vec3(0.5f, 0.0f, 1.0f)));
	barVertices.push_back(Vertex2D(glm::vec2(xMax, yMin), glm::vec3(0.5f, 0.0f, 1.0f)));

	// left corners of bar
	barVertices.push_back(Vertex2D(glm::vec2(xMin, yMax), glm::vec3(1.0f, 0.7f, 0.7f)));
	barVertices.push_back(Vertex2D(glm::vec2(xMin, yMin), glm::vec3(1.0f, 0.7f, 0.7f)));

	// right corners of background
	barVertices.push_back(Vertex2D(glm::vec2(xMax + borderWidth, yMax + borderWidth), glm::vec3(0.7f, 1.0f, 0.7f)));
	barVertices.push_back(Vertex2D(glm::vec2(xMax + borderWidth, yMin - borderWidth), glm::vec3(0.7f, 1.0f, 0.7f)));

	// left corners of background
	barVertices.push_back(Vertex2D(glm::vec2(xMin - borderWidth, yMax + borderWidth), glm::vec3(1.0f, 0.7f, 0.7f)));
	barVertices.push_back(Vertex2D(glm::vec2(xMin - borderWidth, yMin - borderWidth), glm::vec3(1.0f, 0.7f, 0.7f)));

	std::vector<GLuint> barIndices;

	// first bar triangle
	barIndices.push_back(0);
	barIndices.push_back(1);
	barIndices.push_back(2);

	//second bar triangle
	barIndices.push_back(2);
	barIndices.push_back(3);
	barIndices.push_back(1);
	// first background triangle
	barIndices.push_back(4);
	barIndices.push_back(5);
	barIndices.push_back(6);

	//second background triangle
	barIndices.push_back(6);
	barIndices.push_back(7);
	barIndices.push_back(5);

	vertices = barVertices;
	indices = barIndices;

	AdjustVertices(player.windowWidth, player.windowHeight);
	SetVertices();
}

void HealthBarOverlay::Update()
{
	float rightCornerX = player.energy - 0.5f;

	vertices[0] = Vertex2D(glm::vec2(rightCornerX, yMax), glm::vec3(0.5f, 0.0f, 1.0f));
	vertices[1] = Vertex2D(glm::vec2(rightCornerX, yMin), glm::vec3(0.5f, 0.0f, 1.0f));

	AdjustVertex(player.windowWidth, player.windowHeight, vertices[0]);
	AdjustVertex(player.windowWidth, player.windowHeight, vertices[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2), (void*)&vertices[0].position);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex2D), sizeof(glm::vec2), (void*)&vertices[1].position);
}