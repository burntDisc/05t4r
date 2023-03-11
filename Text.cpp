#include "Text.h"

//TODO: Batch Draw Chars
Text::Text(std::string rawText, glm::vec2 origin, float size)
{
	glm::vec2 charOrigin = origin;
	for (int i = 0; i < rawText.size(); ++i)
	{
		char value = rawText.at(i);
		Char nextChar = Char(value, charOrigin, size);
		chars.push_back(nextChar);
		charOrigin.x += size;
	}
}

void Text::Draw(Shader shader)
{
	for (int i = 0; i < chars.size(); ++i)
	{
		chars[i].Draw(shader);
	}
}
