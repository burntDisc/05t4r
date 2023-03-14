#pragma once
#include "Drawable.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

class Overlay:
	public Drawable
{
public:
	Overlay(Shader shader, std::vector<Vertex2D> vertices, std::vector<GLuint> indices);
	Overlay(Shader shader);
	void Draw();
protected:
	GLuint VBO_ID = 0;
	void Refresh();
	std::vector<Vertex2D> vertices;
	std::vector<GLuint> indices;
	VertexArrayObject VAO;
	VertexBufferObject VBO;
	ElementBufferObject EBO;
};

