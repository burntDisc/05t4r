#include "Title.h"
#include "InputHandler.h"

Title::Title(int width, int height):
	Scene(width,height)
{
	shaders.push_back(shader2D);
	Text* gameName = new Text(shader2D, "05t4r", glm::vec2(-0.75f, 0.5f), 0.28f);
	drawables.push_back(gameName);
	deletables.push_back(gameName);

	ipSelector = new IPSelector(shader2D, glm::vec2(-0.7f, 0.0f), 0.09f);
	updatables.push_back(ipSelector);
	drawables.push_back(ipSelector);
	deletables.push_back(ipSelector);

	Text* text = new Text(shader2D, "press start", glm::vec2(-0.5f, -0.5f), 0.09f);
	updatables.push_back(text);
	drawables.push_back(text);
	deletables.push_back(text);

	preLoadedGame = new GamePlay(windowWidth, windowHeight);
}

Scene* Title::Update(double time)
{
	if (InputHandler::state.buttons[GLFW_GAMEPAD_BUTTON_START])
	{
		std::string ip = ipSelector->GetIP();
		//NetworkHandler::StartMatch(ip);
		return preLoadedGame;
	}
	else
	{
	return Scene::Update(time);
}
}

void Title::Draw()
{
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}
