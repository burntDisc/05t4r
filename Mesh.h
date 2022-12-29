#pragma once
#include<string>

#include"VertexArrayObject.h"
#include"ElementBufferObject.h"
#include"Camera.h"
#include"Texture.h"

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	// Store VertexArrayObject in public so it can be used in the Draw function
	VertexArrayObject VAO;

	// number of instances 
	unsigned int instancing;

	// Initializes the mesh
	Mesh
	(
		std::vector <Vertex>& vertices,
		std::vector <GLuint>& indices,
		std::vector <Texture>& textures,
		unsigned int instancing = 1,
		std::vector <glm::mat4> instanceMatrix = {}
	);

	// Draws the mesh
	void Draw
	(
		Shader& shader,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
};