#include "InputHandler.h"
// FOR DEBUG REMOVE
#include<iostream>
//
std::vector<Subscription> InputHandler::subscriptions;
GLFWwindow* InputHandler::window;

void InputHandler::SetWindow(GLFWwindow* newWindow)
{
	window = newWindow;
}

void InputHandler::Subscribe(int GLFWKey, std::function<void(void)> callback)
{
	Subscription newSubscription = {
		GLFWKey,
		callback
	};

	subscriptions.push_back(newSubscription);
}

void InputHandler::ReadandProcessInput()
{
	for (int i = 0; i < subscriptions.size(); ++i)
	{
		int keycode = subscriptions[i].event;
		if (glfwGetKey(window, keycode) == GLFW_PRESS)
		{
			std::cout << "CALLED" << std::endl;
			subscriptions[i].callback();
		}
	}
}