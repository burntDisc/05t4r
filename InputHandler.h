#pragma once
#include <functional>
#include <string>
#include <vector>
#include <functional>
#include "Camera.h"

class InputHandler
{
public:
	enum InputType { keyboard, mouse, joystick };

	struct EventSubscription {
		InputType type;
		int input;
		int event;
		std::function<void(void)> callback;

	};
	struct InputSubscription {
		InputType type;
		int input;
		std::function<void(const float*)> callback;

	};

	static void SetWindow(GLFWwindow* window);
	static void Subscribe(InputType type, int inputSpecifier, int event, std::function<void(void)> callback);
	static void Subscribe(InputType type, int inputSpecifier, std::function<void(const float*)> callback);
	static void ProcessInput();
private:
	static std::vector<EventSubscription> eventSubscriptions;
	static std::vector<InputSubscription> inputSubscriptions;
	static GLFWwindow* window;
};


