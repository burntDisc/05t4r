#pragma once
#include "Updatable.h"
#include "Drawable.h"
#include <vector>

class Scene
{
public:
	~Scene();
	virtual Scene* Update(double time);
	virtual void Draw();
protected:
	Scene* nextScene = nullptr;
	std::vector<Updatable*> updatables;
	std::vector<Drawable*> drawables;
	std::vector<Shader> shaders;
};

