
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

struct Vertex2D
{
	glm::vec2 position;
	glm::vec3 color;
};

class VertexBufferObject
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VertexBufferObject(std::vector<Vertex>& vertices);
	VertexBufferObject(std::vector<Vertex2D>& vertices);
	VertexBufferObject(std::vector<glm::mat4>& mat4s);

	// Binds the VertexBufferObject
	void Bind();
	// Unbinds the VertexBufferObject
	void Unbind();
	// Deletes the VertexBufferObject
	void Delete();
};