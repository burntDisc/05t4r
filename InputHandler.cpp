#include "InputHandler.h"
// FOR DEBUG REMOVE
#include<iostream>
//
std::vector<InputHandler::Subscription> InputHandler::subscriptions;
GLFWwindow* InputHandler::window;

void InputHandler::SetWindow(GLFWwindow* newWindow)
{
	window = newWindow;
}

void InputHandler::Subscribe(InputType type, int inputSpecifier, int event, std::function<void(void)> callback)
{
	Subscription newSubscription = {
		type,
		inputSpecifier,
		event,
		callback
	};

	subscriptions.push_back(newSubscription);
}

void InputHandler::ReadandProcessInput()
{
	for (int i = 0; i < subscriptions.size(); ++i)
	{
		int code = subscriptions[i].input;
		int event = subscriptions[i].event;
		switch (subscriptions[i].type) {
		case keyboard:
			if (glfwGetKey(window, code) == event)
			{
				subscriptions[i].callback();
			}
			break;
		case mouse:
			if (glfwGetMouseButton(window, code) == event)
			{
				subscriptions[i].callback();
			}
			break;
		default:
			break;
		}
	}
}