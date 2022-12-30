#pragma once
#include "GameObject.h"
class ExplodingObject :
	public GameObject
{
public:
	using GameObject::GameObject;
	void Step(float time);
	void Draw(Shader& shader);
private:
	float time = 0;
};

