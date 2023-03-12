#include "Scene.h"

Scene::~Scene()
{

	for (int i = 0; i < shaders.size(); ++i)
	{
		shaders[i].Delete();

	}
	for (int i = 0; i < updatables.size(); ++i)
	{
		delete updatables[i];
	}
	for (int i = 0; i < drawables.size(); ++i)
	{
		delete drawables[i];
	}
}

void Scene::Update(double time)
{
	for (int i = 0; i < updatables.size(); ++i)
	{
		updatables[i]->Update(time);
	}
}

void Scene::Draw()
{
	for (int i = 0; i < drawables.size(); ++i)
	{
		drawables[i]->Draw();
	}
}