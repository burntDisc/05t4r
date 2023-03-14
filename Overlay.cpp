#include "Overlay.h"
#include <glm/gtc/matrix_transform.hpp>

Overlay::Overlay(Shader shader) : Drawable(shader)
{

}

Overlay::Overlay(Shader shader, std::vector<Vertex2D> vertices, std::vector<GLuint> indices) :
	Drawable(shader),
	vertices(vertices),
	indices(indices)
{
	Refresh();
}

void Overlay::Refresh()
{
	VAO.Bind();
	VBO.Update(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO.Update(indices);

	// Links VertexBufferObject attributes such as coordinates and colors to VertexArrayObject

	VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, sizeof(Vertex2D), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex2D), (void*)(2 * sizeof(float)));

	VBO_ID = VBO.ID;

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Overlay::Draw()
{
	// Bind shader to be set uniforms
	shader.Activate();
	VAO.Bind();

	// Draw mesh
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}