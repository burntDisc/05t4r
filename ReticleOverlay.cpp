#include "ReticleOverlay.h"

#include "EnergyBarOverlay.h"


// TEMPLATED ON ENERGY BAR
ReticleOverlay::ReticleOverlay(Shader shader, Player& player, int windowWidth, int windowHeight) :
	windowWidth((float)windowWidth),
	windowHeight((float)windowHeight),
	player(player),
	Overlay::Overlay(shader)
{
	//All triangle vertices are unique and there are 9
	for (int i = 0; i < 9; ++i)
	{
		indices.push_back(i);
	}
	UpdateVertices(0.0f);
	Refresh();
}

void ReticleOverlay::Update(double time)
{
	Updatable::Update(time);
	UpdateVertices(timeDelta);
	Refresh();
	
}

void ReticleOverlay::Delete()
{
	delete this;
}

void ReticleOverlay::UpdateVertices(double delta)
{
	vertices.clear();
	float zoomIncrement = delta * zoomSpeed;
	if (zoomFac + zoomIncrement < (1.0 - player.zoom) / 2.0)
	{
		zoomFac += zoomIncrement;
	}
	else if (zoomFac - zoomIncrement > (1.0 - player.zoom) / 2.0)
	{
		zoomFac -= zoomIncrement;
	}
	float zoomFac = ( 1.0 - player.zoom) / 2.0;
	float radius = zoomFac * (maxRadius - minRadius) + minRadius;
	float rotation = zoomFac * 4.0f * acos(0) - acos(0) / 3.0f; // * 2*PI / (2*PI * 1/6)

	// removing any skew from non-square viewport
	float xAdjustment = 
		windowWidth > windowHeight ? 
		xAdjustment = windowHeight / windowWidth :
		1.0f;

	float yAdjustment =
		yAdjustment =
		windowWidth < windowHeight ?
		windowWidth / windowHeight :
		1.0f;

	for (int i = 0; i < 3; ++i)
	{
		glm::vec2 triangleVert0(
			radius * cos(rotation + (i * 4 * acos(0)) / 3) * xAdjustment,
			radius * sin(rotation + (i * 4 * acos(0)) / 3) * yAdjustment);
		vertices.push_back(Vertex2D(triangleVert0, color));

		glm::vec2 triangleVert1(
			(radius + radialOffset) * cos(rotation + (i * 4 * acos(0)) / 3 + rotationOffset) * xAdjustment,
			(radius + radialOffset) * sin(rotation + (i * 4 * acos(0)) / 3 + rotationOffset) * yAdjustment);
		vertices.push_back(Vertex2D(triangleVert1, color));

		glm::vec2 triangleVert2(
			(radius + radialOffset) * cos(rotation + (i * 4 * acos(0)) / 3 - rotationOffset) * xAdjustment,
			(radius + radialOffset) * sin(rotation + (i * 4 * acos(0)) / 3 - rotationOffset) * yAdjustment);
		vertices.push_back(Vertex2D(triangleVert2, accentColor));
	}
}
