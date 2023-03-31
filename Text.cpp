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

size_t Text::size()
{
	return chars.size();
}
void Text::Draw()
{
	if (visible)
	{
		for (int i = 0; i < chars.size(); ++i)
		{
			chars[i].Draw();
		}
	}
}

void Text::Update(double time)
{
	if (time - lastBlink > blinkInterval)
	{
		lastBlink = time;
		visible = !visible;
	}
}

void Text::ChangeChar(int index, char value)
{
	chars[index].Change(value);
}

char Text::GetChar(int index)
{
	return chars[index].value;
}

std::string Text::GetText()
{
	std::string text = "";
	for (int i = 0; i < chars.size(); ++i)
	{
		text += chars[i].value;
	}

	return text;
}

void Text::ChangeText(std::string newText)
{
	for (int i = 0; i < newText.size(); ++i)
	{
		chars[i].Change(newText.at(i));
	}
}
void Text::Delete()
{
	delete this;
}