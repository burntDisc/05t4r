#include "HealthBarOverlay.h"

//TODO: MAKE IT WORK USING TEMPLATE BELOW
HealthBarOverlay::HealthBarOverlay(Player& player) :
	player(player),
	Overlay::Overlay()
{
	float barOffset = (yMax - yMin) / numBars;
	for (int i = 0; i < numBars; ++i)
	{
		float barYMin = yMin + barOffset * i;
		float barYMax = barYMin + barOffset - barBuffer;
		// left top
		vertices.push_back(Vertex2D(glm::vec2(xMin, barYMax), glm::vec3(0.5f, 0.0f, 1.0f)));
		// left bottom
		vertices.push_back(Vertex2D(glm::vec2(xMin, barYMin), glm::vec3(0.5f, 0.0f, 1.0f)));
		// right top
		vertices.push_back(Vertex2D(glm::vec2(xMax, barYMax), glm::vec3(1.0f, 0.7f, 0.7f)));
		// right bottom
		vertices.push_back(Vertex2D(glm::vec2(xMax, barYMin), glm::vec3(1.0f, 0.7f, 0.7f)));

		// rectangle indices
		// first bar triangle
		indices.push_back(i * 4 + 0);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);

		//second bar triangle
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 3);
		indices.push_back(i * 4 + 1);
	}
	// add background

	// left top
	vertices.push_back(Vertex2D(glm::vec2(xMin - borderWidth, yMax + borderWidth), glm::vec3(0.7f, 1.0f, 0.7f)));
	// left bottom
	vertices.push_back(Vertex2D(glm::vec2(xMin - borderWidth, yMin - borderWidth), glm::vec3(1.0f, 0.7f, 0.7f)));
	// right top
	vertices.push_back(Vertex2D(glm::vec2(xMax + borderWidth, yMax + borderWidth), glm::vec3(0.7f, 1.0f, 0.7f)));
	// right bottom
	vertices.push_back(Vertex2D(glm::vec2(xMax + borderWidth, yMin - borderWidth), glm::vec3(1.0f, 0.7f, 0.7f)));

	// rectangle indices
	// first triangle
	int indiceOffset = numBars * 4;
	indices.push_back(indiceOffset + 0);
	indices.push_back(indiceOffset + 1);
	indices.push_back(indiceOffset + 2);

	//second triangle
	indices.push_back(indiceOffset + 2);
	indices.push_back(indiceOffset + 3);
	indices.push_back(indiceOffset + 1);

	AdjustVertices(player.windowWidth, player.windowHeight);
	SetVertices();
}

void HealthBarOverlay::UpdateIndices(int bars)
{
	indices.clear();
	for (int i = 0; i < bars; ++i)
	{
		// rectangle indices
		// first bar triangle
		indices.push_back(i * 4 + 0);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);

		//second bar triangle
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 3);
		indices.push_back(i * 4 + 1);
	}
	// rectangle indices
	// first triangle
	int indiceOffset = numBars * 4;
	indices.push_back(indiceOffset + 0);
	indices.push_back(indiceOffset + 1);
	indices.push_back(indiceOffset + 2);

	//second triangle
	indices.push_back(indiceOffset + 2);
	indices.push_back(indiceOffset + 3);
	indices.push_back(indiceOffset + 1);
}

void HealthBarOverlay::Update()
{
	indices.clear();
	int bars = (int)(player.health / 100.0f * numBars);
	UpdateIndices(bars);
}