#pragma once
#include "Updatable.h"
#include "Drawable.h"
#include "Deletable.h"
#include <vector>

class Scene
{
public:
	Scene(int width, int height);
	~Scene();
	virtual Scene* Update(double time);
	virtual void Draw();
protected:
	int windowWidth, windowHeight;
	Scene* nextScene = nullptr;
	std::vector<Updatable*> updatables;
	std::vector<Drawable*> drawables;
	std::vector<Deletable*> deletables;
	std::vector<Shader> shaders;
};

