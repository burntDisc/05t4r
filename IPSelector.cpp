#include "IPSelector.h"
#include "InputHandler.h"

#include <cmath>

IPSelector::IPSelector(Shader shader, glm::vec2 origin, float size) :
	Deletable(),
	origin(origin),
	size(size),
	Overlay(shader),
	ip("192.168.001.108"),
	text(shader, "192.168.001.108", origin, size)
{
	float height = size;
	float width = ip.size() * size;
	//selector
	int index = (int)activeDigit;

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

	vertices.push_back(Vertex2D(origin,							   glm::vec3(1.0f, 0.7f, 0.7f)));
	vertices.push_back(Vertex2D(origin + glm::vec2(0.0f, height),  glm::vec3(0.7f, 1.0f, 0.7f)));
	vertices.push_back(Vertex2D(origin + glm::vec2(width, height), glm::vec3(1.0f, 0.7f, 0.7f))); 
	vertices.push_back(Vertex2D(origin + glm::vec2(width, 0.0f),   glm::vec3(0.7f, 1.0f, 0.7f)));

	Refresh();
}

int IPSelector::GetStringOffset(int index)
{
	int numDots = activeDigit / 3;
	return numDots + activeDigit;
}

void IPSelector::ShiftDigit(int activeDigit)
{
	int stringIndex = GetStringOffset(activeDigit);
	ip[stringIndex] = characterOffset;

	if (ip[stringIndex] > '9')
	{
		int adjustedShift = (ip[stringIndex] - '0') % 10;
		ip[stringIndex] = '0' + adjustedShift;
	}
	else if(ip[stringIndex] < '0')
	{
		int adjustedShift = (ip[stringIndex] - '0') % 10;
		ip[stringIndex] = '9' + adjustedShift;
	}
	text.ChangeChar(stringIndex, ip[stringIndex]);
}

void IPSelector::UpdateSelector()
{
	int stringIndex = GetStringOffset(activeDigit);
	vertices[0] = Vertex2D(origin + glm::vec2(size * stringIndex, size), selectorColor);
	vertices[1] = Vertex2D(origin + glm::vec2(size * stringIndex, 0.0f), selectorColor);
	vertices[2] = Vertex2D(origin + glm::vec2(size * (stringIndex + 1), size), selectorColor);
	vertices[3] = Vertex2D(origin + glm::vec2(size * (stringIndex + 1), 0.0f), selectorColor);

	Refresh();
}

void IPSelector::Update(double time)
{
	Updatable::Update(time);
	float xAxis = InputHandler::state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
	float yAxis = InputHandler::state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
	InputHandler::ScaleAxis(xAxis, yAxis);

	int startingIndex = (int)activeDigit;
	// Handle Digit Selection from input
	if (abs(xAxis) > abs(yAxis) && xAxis > 0.0f)
	{
		if (digitSelectSpeed <= 0.0f)
		{
			digitSelectSpeed = 0.0f;
			++activeDigit;
		}
		digitSelectSpeed += digitSelectAccel * timeDelta;

	} 
	else if (abs(xAxis) > abs(yAxis))
	{
		if (digitSelectSpeed >= 0.0f)
		{
			digitSelectSpeed = 0.0f;
			--activeDigit;
		}
		digitSelectSpeed -= digitSelectAccel * timeDelta;
	}
	else
	{
		digitSelectSpeed = 0.0f;
	}

	// Handle digit shifing from input
	if (abs(yAxis) > abs(xAxis) && yAxis < 0.0f)
	{
		if (digitShiftSpeed <= 0.0f)
		{
			digitShiftSpeed = 0.0f;
			++characterOffset;
		}
		digitShiftSpeed += digitShiftAccel * timeDelta;
	}
	else if (abs(yAxis) > abs(xAxis))
	{
		if (digitShiftSpeed >= 0.0f)
		{
			digitShiftSpeed = 0.0f;
			--characterOffset;
		}
		digitShiftSpeed -= digitShiftAccel * timeDelta;
	}
	else
	{
		digitShiftSpeed = 0.0f;
	}

	// do select
	activeDigit += digitSelectSpeed * timeDelta;

	// loop digit values in bouonds
	float stringSize = 12.0f;
	if (activeDigit > stringSize)
	{
		activeDigit -= stringSize;
	}
	else if (activeDigit < 0.0f)
	{
		activeDigit += stringSize;
	}

	// update characterOffset on index change
	int currentIndex = (int)activeDigit;
	if (startingIndex != currentIndex)
	{
		int stringIndex = GetStringOffset(currentIndex);
		characterOffset = ip[stringIndex];
	}
	// Do shift
	else
	{
		characterOffset += digitShiftSpeed * timeDelta;
		ShiftDigit(currentIndex);
	}

	UpdateSelector();
}

void IPSelector::Delete()
{
	delete this;
}

std::string IPSelector::GetIP()
{
	// trim leading 0's
	std::string parsedIP = ip;
	for (int i = 0; i < parsedIP.size(); ++i)
	{
		if (parsedIP[i] == '.' && parsedIP[i + 1] == '0')
		{
			parsedIP.erase(parsedIP.begin() + i + 1);
			--i;
		}
	}
	return parsedIP;
}

void IPSelector::Draw()
{
	text.Draw();
	Overlay::Draw();
}
