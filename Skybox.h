#pragma once
#include <string>
#include "Shader.h"

class Skybox
{
public:
	Skybox(std::string facesDirectory);
	void Draw(Shader skyboxShader);
private:
	float skyboxVertices[24];
	unsigned int skyboxIndices[36];
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO, cubemapTexture;
};

