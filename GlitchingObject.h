#pragma once
#include "GameObject.h"
#include "Shader.h"

class GlitchingObject :
    public GameObject
{
public:
	using GameObject::GameObject;
	void Update(float phase);
	void Draw(Shader& shader);
private:
	float level = -1.0f;
	float factor = -1.0f;
	float prevCycle = 0.0f;
	const float amplitude = 1.0f;
	const float speed = 2.0f;
};
