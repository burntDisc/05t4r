#include"VertexBufferObject.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VertexBufferObject::VertexBufferObject(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VertexBufferObject::VertexBufferObject(std::vector<Vertex2D>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), vertices.data(), GL_STATIC_DRAW);
}

VertexBufferObject::VertexBufferObject(std::vector<glm::mat4>& mat4s)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, mat4s.size() * sizeof(glm::mat4), mat4s.data(), GL_STATIC_DRAW);
}

// Binds the VertexBufferObject
void VertexBufferObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VertexBufferObject
void VertexBufferObject::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VertexBufferObject
void VertexBufferObject::Delete()
{
	glDeleteBuffers(1, &ID);
}