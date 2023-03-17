#include "TextInput.h"
#include "InputHandler.h"

TextInput::TextInput(Shader shader, glm::vec2 origin, float size, int numChars) :
	Deletable(),
	origin(origin),
	size(size),
	Overlay(shader)
{
	std::string initString = "";
	for (int i = 0; i < numChars; ++i)
	{
		initString += " ";
	}
	text = new Text(shader, initString, origin, size);

	float height = size;
	float width = numChars * size; // length of ip string
	//selector
	int index = (int)activeChar;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(1);

	vertices.push_back(Vertex2D(origin + glm::vec2(size * index, height), selectorColor));
	vertices.push_back(Vertex2D(origin + glm::vec2(size * index, 0.0f), selectorColor));
	vertices.push_back(Vertex2D(origin + glm::vec2(size * (index + 1), height), selectorColor));
	vertices.push_back(Vertex2D(origin + glm::vec2(size * (index + 1), 0.0f), selectorColor));

	//background 
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(4);

	vertices.push_back(Vertex2D(origin, glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(origin + glm::vec2(0.0f, height), glm::vec3(0.7f, 1.0f, 0.7f)));
	vertices.push_back(Vertex2D(origin + glm::vec2(width, height), glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(origin + glm::vec2(width, 0.0f), glm::vec3(0.7f, 1.0f, 0.7f)));

	Refresh();
}

TextInput::~TextInput()
{
	delete text;
}

void TextInput::ShiftDigit(int index)
{
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
	text->ChangeChar(index, newChar);
}

void TextInput::UpdateGraphics(int index)
{
	if (index == -1)
	{
		index = (int)activeChar;
	}

	if (selectedIndex != index)
	{
		characterOffset = text->GetChar(index);
		charShiftSpeed = 0.0f;

		vertices[0] = Vertex2D(origin + glm::vec2(size * index, size), selectorColor);
		vertices[1] = Vertex2D(origin + glm::vec2(size * index, 0.0f), selectorColor);
		vertices[2] = Vertex2D(origin + glm::vec2(size * (index + 1), size), selectorColor);
		vertices[3] = Vertex2D(origin + glm::vec2(size * (index + 1), 0.0f), selectorColor);
		Refresh();

		selectedIndex = index;
	}
	else
	{
		characterOffset += charShiftSpeed * timeDelta;
		ShiftDigit(selectedIndex);
	}
}

void TextInput::Update(double time)
{
	Updatable::Update(time);
	float xAxis = InputHandler::state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
	float yAxis = InputHandler::state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
	InputHandler::ScaleAxis(xAxis, yAxis);

	// Handle Digit Selection from input
	if (abs(xAxis) > abs(yAxis) && xAxis > 0.0f)
	{
		if (charSelectSpeed <= 0.0f)
		{
			charSelectSpeed = 0.0f;
			++activeChar;
		}
		charSelectSpeed += charSelectAccel * timeDelta;

	}
	else if (abs(xAxis) > abs(yAxis))
	{
		if (charSelectSpeed >= 0.0f)
		{
			charSelectSpeed = 0.0f;
			--activeChar;
		}
		charSelectSpeed -= charSelectAccel * timeDelta;
	}
	else
	{
		charSelectSpeed = 0.0f;
	}

	// Handle digit shifing from input
	if (abs(yAxis) > abs(xAxis) && yAxis < 0.0f)
	{
		if (charShiftSpeed <= 0.0f)
		{
			charShiftSpeed = 0.0f;
			++characterOffset;
		}
		charShiftSpeed += charShiftAccel * timeDelta;
	}
	else if (abs(yAxis) > abs(xAxis))
	{
		if (charShiftSpeed >= 0.0f)
		{
			charShiftSpeed = 0.0f;
			--characterOffset;
		}
		charShiftSpeed -= charShiftAccel * timeDelta;
	}
	else
	{
		charShiftSpeed = 0.0f;
	}

	// do select
	activeChar += charSelectSpeed * timeDelta;

	// loop digit values in bounds

	float stringSize = text->size();
	if (activeChar > stringSize)
	{
		activeChar -= stringSize;
	}
	else if (activeChar < 0.0f)
	{
		activeChar += stringSize;
	}

	// update characterOffset on index change

	UpdateGraphics();
}

void TextInput::Delete()
{
	delete this;
}

std::string TextInput::GetText()
{
	return text->GetText();
}

void TextInput::Draw()
{
	text->Draw();
	Overlay::Draw();
}
