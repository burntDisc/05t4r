#pragma once
#include <functional>
#include <string>
#include <vector>
#include <functional>
#include "Camera.h"

class InputHandler
{
public:
	enum InputType { keyboard, mouse };

	struct Subscription {
		InputType type;
		int input;
		int event;
		std::function<void(void)> callback;

	};

	static void SetWindow(GLFWwindow* window);
	static void Subscribe(InputType type, int inputSpecifier, int event, std::function<void(void)> callback);
	static void ReadandProcessInput();
private:
	static std::vector<Subscription> subscriptions;
	static GLFWwindow* window;
};


