#pragma once

#include "GameObject.h"
#include "Shader.h"

class ExplodingObject :
	public GameObject
{
public:
	void Update(double time);
	void Draw(Shader& shader);
private:
	float phase;
	float amplitude = 1.0f;
	float speed = 1.0f;
};

