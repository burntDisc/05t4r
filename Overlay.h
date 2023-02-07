#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include "VertexArrayObject.h"
class Overlay
{
public:
	Overlay(std::vector<Vertex2D> vertices, std::vector<GLuint> indices);
	Overlay();

	void AdjustVertices(float windowWidth, float windowHeight);
	void AdjustVertex(float windowWidth, float windowHeight, Vertex2D& vertex);
	void Draw(Shader shader);
protected:
	GLuint VBO_ID = 0;
	void SetVertices();
	std::vector<Vertex2D> vertices;
	std::vector<GLuint> indices;
	VertexArrayObject VAO;
};

