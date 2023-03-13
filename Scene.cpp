#include "Scene.h"

Scene::Scene(int width, int height):
	windowWidth(width), windowHeight(height)
{
}

Scene::~Scene()
{
	for (int i = 0; i < shaders.size(); ++i)
	{
		shaders[i].Delete();

	}

	for (int i = 0; i < deletables.size(); ++i)
	{
		deletables[i]->Delete();
	}
}

Scene* Scene::Update(double time)
{
	for (int i = 0; i < updatables.size(); ++i)
	{
		updatables[i]->Update(time);
	}
	return nextScene;
}

void Scene::Draw()
{
	for (int i = 0; i < drawables.size(); ++i)
	{
		drawables[i]->Draw();
	}
}