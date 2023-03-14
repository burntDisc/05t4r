#include "EnergyBarOverlay.h"

EnergyBarOverlay::EnergyBarOverlay(Shader shader, Player& player):
	player(player),
	Overlay::Overlay(shader)
{
	std::vector<Vertex2D> barVertices;

	// right corners of bar
	barVertices.push_back(Vertex2D(glm::vec2(xMax, yMax), glm::vec3(0.5f, 0.0f, 1.0f)));
	barVertices.push_back(Vertex2D(glm::vec2(xMax, yMin), glm::vec3(0.5f, 0.0f, 1.0f)));

	// left corners of bar
	barVertices.push_back(Vertex2D(glm::vec2(xMin, yMax), glm::vec3(1.0f, 0.7f, 0.7f)));
	barVertices.push_back(Vertex2D(glm::vec2(xMin, yMin), glm::vec3(1.0f, 0.7f, 0.7f)));

	// right corners of background
	barVertices.push_back(Vertex2D(glm::vec2(xMax + borderWidth, yMax + borderWidth), glm::vec3(0.7f, 0.8f, 0.4f)));
	barVertices.push_back(Vertex2D(glm::vec2(xMax + borderWidth, yMin - borderWidth), glm::vec3(0.7f, 1.0f, 0.7f)));

	// left corners of background
	barVertices.push_back(Vertex2D(glm::vec2(xMin - borderWidth, yMax + borderWidth), glm::vec3(1.0f, 0.7f, 0.7f)));
	barVertices.push_back(Vertex2D(glm::vec2(xMin - borderWidth, yMin - borderWidth), glm::vec3(1.0f, 0.7f, 0.7f)));

	std::vector<GLuint> barIndices;
	// first background triangle
	barIndices.push_back(4);
	barIndices.push_back(5);
	barIndices.push_back(6);

	//second background triangle
	barIndices.push_back(6);
	barIndices.push_back(7);
	barIndices.push_back(5);

	// first bar triangle
	barIndices.push_back(0);
	barIndices.push_back(1);
	barIndices.push_back(2);

	//second bar triangle
	barIndices.push_back(2);
	barIndices.push_back(3);
	barIndices.push_back(1);

	vertices = barVertices;
	indices = barIndices;

	Refresh();
}

void EnergyBarOverlay::Update(double time)
{
	float rightCornerX = player.energy * (xMax - xMin) + xMin;

	vertices[0] = Vertex2D(glm::vec2(rightCornerX, yMax), glm::vec3(0.7f, 0.8f, 0.4f));
	vertices[1] = Vertex2D(glm::vec2(rightCornerX, yMin), glm::vec3(0.7f, 0.8f, 0.4f));

	Refresh();
}

void EnergyBarOverlay::Delete()
{
	delete this;
}