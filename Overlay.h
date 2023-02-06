#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include "VertexArrayObject.h"
class Overlay
{
public:
	Overlay(std::vector<Vertex2D> vertices, std::vector<GLuint> indices);
	void Draw(Shader shader);
private:
	std::vector<Vertex2D> vertices;
	std::vector<GLuint> indices;
	VertexArrayObject VAO;
};

