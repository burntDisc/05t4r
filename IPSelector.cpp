#include "IPSelector.h"
#include "InputHandler.h"

#include <cmath>

IPSelector::IPSelector(Shader shader, glm::vec2 origin, float size) :
	TextInput(shader, origin, size, 15)
{
	text->ChangeText("002.168.001.108");
}

int IPSelector::GetStringOffset(int index)
{
	int numDots = activeChar / 3;
	return numDots + activeChar;
}

void IPSelector::ShiftDigit(int index)
{
	int stringIndex = GetStringOffset(index);
	char newChar = characterOffset;

	if (newChar > '9')
	{
		int adjustedShift = (newChar - '0') % 10;
		newChar = '0' + adjustedShift;
	}
	else if (newChar < '0')
	{
		int adjustedShift = (newChar - '0') % 10;
		newChar = '9' + adjustedShift;
	}
	text->ChangeChar(stringIndex, newChar);
}

void IPSelector::UpdateGraphics(int index)
{
	float stringSize = 12; // chars in ip
	if (activeChar > stringSize)
	{
		activeChar -= stringSize;
	}
	else if (activeChar < 0.0f)
	{
		activeChar += stringSize;
	}
	int charSelected = GetStringOffset(index);
	TextInput::UpdateGraphics(charSelected);
}

std::string IPSelector::GetIP()
{
	// trim leading 0's
	std::string parsedIP = text->GetText();
	bool leadingZeroes = true;
	for (int i = 0; i < parsedIP.size(); ++i)
	{
		if (leadingZeroes && parsedIP[i] == '0')
		{
			parsedIP.erase(parsedIP.begin() + i);
			--i;
		}
		else
		{
			leadingZeroes = false;
		}

		if (parsedIP[i] == '.' && parsedIP[i + 1] == '0')
		{
			parsedIP.erase(parsedIP.begin() + i + 1);
			--i;
		}
	}
	return parsedIP;
}