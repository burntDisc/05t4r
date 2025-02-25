#include "Skybox.h"

#include<stb/stb_image.h>
#include <glad/glad.h>
#include <iostream>

Skybox::Skybox(Shader shader, std::string facesDirectory):
	Drawable(shader),
	skyboxVertices{
	//   Coordinates
	  -1.0f,-1.0f, 1.0f,//        7--------6
	   1.0f,-1.0f, 1.0f,//       /|       /|
	   1.0f,-1.0f,-1.0f,//      4--------5 |
	  -1.0f,-1.0f,-1.0f,//      | |      | |
	  -1.0f, 1.0f, 1.0f,//      | 3------|-2
	   1.0f, 1.0f, 1.0f,//      |/       |/
	   1.0f, 1.0f,-1.0f,//      0--------1
	  -1.0f, 1.0f,-1.0f
	},
	skyboxIndices
	{
			// Right
			1, 2, 6,
			6, 5, 1,
			// Left
			0, 4, 7,
			7, 3, 0,
			// Top
			4, 5, 6,
			6, 7, 4,
			// Bottom
			0, 3, 2,
			2, 1, 0,
			// Back
			0, 1, 5,
			5, 4, 0,
			// Front
			3, 7, 6,
			6, 2, 3
	}
{
	// Create VertexArrayObject, VertexBufferObject, and ElementBufferObject for the skybox
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// All the faces of the cubemap in order
	std::string skycubeTexturePaths[6] =
	{
		facesDirectory + "right.png",
		facesDirectory + "left.png",
		facesDirectory + "top.png",
		facesDirectory + "bottom.png",
		facesDirectory + "front.png",
		facesDirectory + "back.png",
	};

	// Creates the cubemap texture object
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(skycubeTexturePaths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << skycubeTexturePaths[i] << std::endl;
			stbi_image_free(data);
		}
	}

}

// Draw skybox last to optimize fragment pass
void Skybox::Draw()
{

	// Setup and Draw skybox--------------------------
	// set constant depth for skybox
	shader.Activate();
	//Uniform for texture access format (see shader)
	glUniform1i(glGetUniformLocation(shader.ID, "skybox"), 0);

	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Skybox::Delete()
{
	delete this;
}
