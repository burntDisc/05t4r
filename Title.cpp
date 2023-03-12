#include "Title.h"

Title::Title() :
	ipSelector(shader2D, glm::vec2(-0.5f, 0.0f), 0.05f)
{
	shaders.push_back(shader2D);
}

void Title::Draw()
{
	ipSelector.Draw();
}
