#pragma once

#include "GameObject.h"
#include "Shader.h"

class ExplodingObject :
	public GameObject
{
public:
	using GameObject::GameObject;
	void Set(double phase);
	void Draw(Shader& shader);
private:
	float phase;
	float amplitude = 1;
	float speed = 1;
};

