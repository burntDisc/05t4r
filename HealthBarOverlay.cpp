#include "HealthBarOverlay.h"

//TODO: MAKE IT WORK USING TEMPLATE BELOW
HealthBarOverlay::HealthBarOverlay(Shader shader, Player& player) :
	player(player),
	Overlay::Overlay(shader)
{
	float barOffset = (yMax - yMin) / numBars;


	// background rectangle indices
	// first triangle
	int indiceOffset = numBars * 4;
	indices.push_back(indiceOffset + 0);
	indices.push_back(indiceOffset + 1);
	indices.push_back(indiceOffset + 2);

	//second triangle
	indices.push_back(indiceOffset + 2);
	indices.push_back(indiceOffset + 3);
	indices.push_back(indiceOffset + 1);

	// assemble bars
	for (int i = 0; i < numBars; ++i)
	{
		float barYMin = yMin + barOffset * i;
		float barYMax = barYMin + barOffset - barBuffer;
		// left top
		vertices.push_back(Vertex2D(glm::vec2(xMin, barYMax), color0));
		// left bottom
		vertices.push_back(Vertex2D(glm::vec2(xMin, barYMin), color0));
		// right top
		vertices.push_back(Vertex2D(glm::vec2(xMax, barYMax), color1));
		// right bottom
		vertices.push_back(Vertex2D(glm::vec2(xMax, barYMin), color1));

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


	// add background vertices

	// left top
	vertices.push_back(Vertex2D(glm::vec2(xMin - borderWidth, yMax + borderWidth), color2));
	// left bottom
	vertices.push_back(Vertex2D(glm::vec2(xMin - borderWidth, yMin - borderWidth), color1));
	// right top
	vertices.push_back(Vertex2D(glm::vec2(xMax + borderWidth, yMax + borderWidth), color2));
	// right bottom
	vertices.push_back(Vertex2D(glm::vec2(xMax + borderWidth, yMin - borderWidth), color1));

	Refresh();
}

void HealthBarOverlay::UpdateIndices(int bars)
{
	indices.clear();

	// background
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

	// bars
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
	Refresh();
}

void HealthBarOverlay::Update(double time)
{
	indices.clear();
	int bars = (int)(player.health / 100.0f * numBars);
	UpdateIndices(bars);
}

void HealthBarOverlay::Delete()
{
	delete this;
}
