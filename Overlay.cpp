#include "Overlay.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include <glm/gtc/matrix_transform.hpp>

Overlay::Overlay(){}

Overlay::Overlay(std::vector<Vertex2D> vertices, std::vector<GLuint> indices) :
	vertices(vertices),
	indices(indices)
{
	SetVertices();
}

void Overlay::SetVertices()
{
	VAO.Bind();
	VertexBufferObject VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	ElementBufferObject EBO(indices);

	// Links VertexBufferObject attributes such as coordinates and colors to VertexArrayObject
	VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, sizeof(Vertex2D), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex2D), (void*)(2 * sizeof(float)));

	VBO_ID = VBO.ID;

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

//TODO OPTIMIZATION: move both below adjustment functions to shader
void Overlay::AdjustVertices(float windowWidth, float windowHeight)
{
	for (int i = 0; i < vertices.size(); ++i)
	{
		glm::mat4 persepctive = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);

		glm::vec4 vec;
		vec.x = vertices[i].position.x;
		vec.y = vertices[i].position.y;
		vec.z = 0.0;

		vec = persepctive * vec;
		vertices[i].position.x = vec.x;
		vertices[i].position.y = vec.y;

	}
}

void Overlay::AdjustVertex(float windowWidth, float windowHeight, Vertex2D& vertex)
{
	glm::mat4 persepctive = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);

	glm::vec4 vec;
	vec.x = vertex.position.x;
	vec.y = vertex.position.y;
	vec.z = 0.0;

	vec = persepctive * vec;
	vertex.position.x = vec.x;
	vertex.position.y = vec.y;
}

void Overlay::Draw(Shader shader)
{
	// Bind shader to be set uniforms
	shader.Activate();
	VAO.Bind();

	// Draw mesh
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}