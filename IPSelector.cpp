#include "IPSelector.h"

IPSelector::IPSelector(Shader shader, glm::vec2 origin, float size) :
	Overlay(shader),
	ip("000.000.000.000"),
	text(shader, "000.000.000.000", glm::vec2(-0.5f, 0.0f), 0.05f)
{
	float height = size;
	float width = ip.size() * size;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(1);

	vertices.push_back(Vertex2D(origin + glm::vec2(0.0f, height),  glm::vec3(0.7f, 1.0f, 0.7f)));
	vertices.push_back(Vertex2D(origin,							   glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(origin + glm::vec2(width, height), glm::vec3(0.7f, 1.0f, 0.7f)));
	vertices.push_back(Vertex2D(origin + glm::vec2(width, 0.0f),   glm::vec3(1.0f, 0.7f, 0.7f)));

	SetVertices();
}

void IPSelector::Update(double time)
{
	// TODO

}

void IPSelector::Draw()
{
	text.Draw();
	Overlay::Draw();
}
