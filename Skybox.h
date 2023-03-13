#pragma once

#include <string>

#include "Drawable.h"
#include "Shader.h"
#include "Deletable.h"

class Skybox:
	public Drawable,
	public Deletable
{
public:
	Skybox(Shader shader, std::string facesDirectory);
	void Draw();
	void Delete();
private:
	float skyboxVertices[24];
	unsigned int skyboxIndices[36];
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO, cubemapTexture;
};

