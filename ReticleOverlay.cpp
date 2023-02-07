#include "ReticleOverlay.h"

#include "EnergyBarOverlay.h"

// TODO: MAKE IT WORK!!
// TEMPLATED ON ENERGY BAR
ReticleOverlay::ReticleOverlay(Player& player) :
	player(player),
	Overlay::Overlay()
{
	//All triangle vertices are unique and there are 9
	for (int i = 0; i < 9; ++i)
	{
		indices.push_back(i);
	}
	UpdateVertices();
	SetVertices();
}

void ReticleOverlay::Update()
{
	UpdateVertices();

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	//reset vertex positions
	for (int i = 0; i < 9; ++i)
	{
		glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(Vertex2D), sizeof(Vertex2D), (void*)&vertices[i]);
	}
}

void ReticleOverlay::UpdateVertices()
{
	vertices.clear();
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

	glm::vec3 accentColor = player.targetLocked ?
		glm::vec3(0.0f, 0.0f, 1.0f):
		glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec2 triangleAVert0(
		radius * cos(rotation),
		radius * sin(rotation));
	glm::vec2 triangleAVert1(
		(radius + radialOffset) * cos(rotation + rotationOffset),
		(radius + radialOffset) * sin(rotation + rotationOffset));
	glm::vec2 triangleAVert2(
		(radius + radialOffset) * cos(rotation - rotationOffset),
		(radius + radialOffset) * sin(rotation - rotationOffset));
	vertices.push_back(Vertex2D(triangleAVert0, glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(triangleAVert1, glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(triangleAVert2, accentColor));

	glm::vec2 triangleBVert0(
		radius* cos(rotation + (4 * acos(0)) / 3),
		radius * sin(rotation + (4 * acos(0)) / 3));
	glm::vec2 triangleBVert1(
		(radius + radialOffset) * cos(rotation + (4 * acos(0)) / 3 + rotationOffset),
		(radius + radialOffset) * sin(rotation + (4 * acos(0)) / 3 + rotationOffset));
	glm::vec2 triangleBVert2(
		(radius + radialOffset) * cos(rotation + (4 * acos(0)) / 3 - rotationOffset),
		(radius + radialOffset) * sin(rotation + (4 * acos(0)) / 3 - rotationOffset));

	vertices.push_back(Vertex2D(triangleBVert0, glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(triangleBVert1, glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(triangleBVert2, accentColor));

	glm::vec2 triangleCVert0(
		radius* cos(rotation + (8 * acos(0)) / 3),
		radius * sin(rotation + (8 * acos(0)) / 3));
	glm::vec2 triangleCVert1(
		(radius + radialOffset) * cos(rotation + (8 * acos(0)) / 3 + rotationOffset),
		(radius + radialOffset) * sin(rotation + (8 * acos(0)) / 3 + rotationOffset));
	glm::vec2 triangleCVert2(
		(radius + radialOffset) * cos(rotation + (8 * acos(0)) / 3 - rotationOffset),
		(radius + radialOffset) * sin(rotation + (8 * acos(0)) / 3 - rotationOffset));

	vertices.push_back(Vertex2D(triangleCVert0, glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(triangleCVert1, glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(triangleCVert2, accentColor));
	AdjustVertices(player.windowWidth, player.windowHeight);
}
