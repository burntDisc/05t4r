#pragma once
#include "GameObject.h"
class ExplodingObject :
	public GameObject
{
public:
	using GameObject::GameObject;
	void Set(float phase);
	void Draw(Shader& shader);
private:
	float phase = 0;
	float amplitude = 1;
	float speed = 1;
};

