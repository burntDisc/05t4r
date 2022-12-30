#pragma once
#include <functional>
#include <string>
#include <vector>
#include <functional>
#include "Camera.h"

struct Subscription {
	int event;
	std::function<void(void)> callback;

};

class InputHandler
{
public:

	static void SetWindow(GLFWwindow* window);
	static void Subscribe(int GLFWKey, std::function<void(void)> callback);
	static void ReadandProcessInput();
private:
	static std::vector<Subscription> subscriptions;
	static GLFWwindow* window;
};


