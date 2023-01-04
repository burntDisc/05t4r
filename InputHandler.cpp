#include "InputHandler.h"
// FOR DEBUG REMOVE
#include<iostream>
//
std::vector<InputHandler::EventSubscription> InputHandler::eventSubscriptions;
std::vector<InputHandler::InputSubscription> InputHandler::inputSubscriptions;
GLFWwindow* InputHandler::window;

void InputHandler::SetWindow(GLFWwindow* newWindow)
{
	window = newWindow;
}

void InputHandler::Subscribe(InputType type, int inputSpecifier, int event, std::function<void(void)> callback)
{
	EventSubscription newSubscription = {
		type,
		inputSpecifier,
		event,
		callback
	};

	eventSubscriptions.push_back(newSubscription);
}

void InputHandler::Subscribe(InputType type, int inputSpecifier, std::function<void(const float*)> callback)
{
	InputSubscription newSubscription = {
		type,
		inputSpecifier,
		callback
	};

	inputSubscriptions.push_back(newSubscription);
}

void InputHandler::ProcessInput()
{
	for (int i = 0; i < eventSubscriptions.size(); ++i)
	{
		int code = eventSubscriptions[i].input;
		int event = eventSubscriptions[i].event;
		switch (eventSubscriptions[i].type) {
		case keyboard:
			if (glfwGetKey(window, code) == event)
			{
				eventSubscriptions[i].callback();
			}
			break;
		case mouse:
			if (glfwGetMouseButton(window, code) == event)
			{
				eventSubscriptions[i].callback();
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < inputSubscriptions.size(); ++i)
	{
		int code = inputSubscriptions[i].input;
		switch (inputSubscriptions[i].type) {
		case joystick:
		{
			int count = 2;
			const float* axes = glfwGetJoystickAxes(code, &count);
			if (axes)
			{
				inputSubscriptions[i].callback(axes);
			}
		}
		break;
		default:
			break;
		}
	}
}
