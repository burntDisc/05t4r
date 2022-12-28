#include"VertexArrayObject.h"

// Constructor that generates a VertexArrayObject ID
VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &ID);
}

// Links a VertexBufferObject Attribute such as a position or color to the VertexArrayObject
void VertexArrayObject::LinkAttrib(VertexBufferObject& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VertexArrayObject
void VertexArrayObject::Bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VertexArrayObject
void VertexArrayObject::Unbind()
{
	glBindVertexArray(0);
}

// Deletes the VertexArrayObject
void VertexArrayObject::Delete()
{
	glDeleteVertexArrays(1, &ID);
}