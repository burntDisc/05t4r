#include "Text.h"

//TODO: Batch Draw Chars
Text::Text(Shader shader, std::string rawText, glm::vec2 origin, float size) : Overlay(shader)
{
	glm::vec2 charOrigin = origin;
	for (int i = 0; i < rawText.size(); ++i)
	{
		char value = rawText.at(i);
		Char nextChar = Char(shader, value, charOrigin, size);
		chars.push_back(nextChar);
		charOrigin.x += size;
	}
}

void Text::Draw()
{
	for (int i = 0; i < chars.size(); ++i)
	{
		chars[i].Draw();
	}
}
