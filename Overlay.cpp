#include "Overlay.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

Overlay::Overlay(std::vector<Vertex2D> vertices, std::vector<GLuint> indices):
	vertices(vertices),
	indices(indices)
{
	VAO.Bind();
	VertexBufferObject VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	ElementBufferObject EBO(indices);
	// Links VertexBufferObject attributes such as coordinates and colors to VertexArrayObject
	VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, sizeof(Vertex2D), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex2D), (void*)(2 * sizeof(float)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Overlay::Draw(Shader shader)
{
	// Bind shader to be set uniforms
	shader.Activate();
	VAO.Bind();

	// Draw mesh
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}