#pragma once
#include "Updatable.h"
#include "Drawable.h"
#include <vector>

class Scene :
	public Updatable
{
public:
	~Scene();
	void Update(double time);
	void Draw();
protected:
	std::vector<Updatable*> updatables;
	std::vector<Drawable*> drawables;
	std::vector<Shader> shaders;
};

