#include "Title.h"

Title::Title() :
	text(std::string("05t4r"), glm::vec2(-1.0, 0.0), 0.09)
{
}

void Title::Draw(Shader shader)
{
	text.Draw(shader);
}
