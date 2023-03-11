#include "Title.h"

Title::Title() :
	ipSelector(glm::vec2(-0.5f, 0.0f), 0.05f)
{
}

void Title::Draw(Shader shader)
{
	ipSelector.Draw(shader);
}
