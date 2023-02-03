#include "InputHandler.h"

std::vector<InputHandler::EventSubscription> InputHandler::eventSubscriptions;
std::vector<InputHandler::InputSubscription> InputHandler::inputSubscriptions;
GLFWwindow* InputHandler::window;
int cyclesPassed = 0;
void InputHandler::SetWindow(GLFWwindow* newWindow)
{
	window = newWindow;
}

void InputHandler::Subscribe(InputType type, int inputSpecifier, int event, std::function<void(void)> pressCallback, std::function<void(void)> releaseCallback)
{
	EventSubscription newSubscription = {
		type,
		inputSpecifier,
		event,
		pressCallback,
		releaseCallback,
	};

	eventSubscriptions.push_back(newSubscription);
}

void InputHandler::Subscribe(InputType type, int inputSpecifier, int index, std::function<void(float*)> callback)
{
	InputSubscription newSubscription = {
		type,
		inputSpecifier,
		index,
		callback
	};

	inputSubscriptions.push_back(newSubscription);
}

void InputHandler::ProcessInput()
{
	// button logic
	for (int i = 0; i < eventSubscriptions.size(); ++i)
	{
		int code = eventSubscriptions[i].input;
		int event = eventSubscriptions[i].event;
		switch (eventSubscriptions[i].type) {
		case keyboard:
			if (glfwGetKey(window, code) == event)
			{
				eventSubscriptions[i].pressCallback();
			}
			break;
		case mouse:
			if (glfwGetMouseButton(window, code) == event)
			{
				eventSubscriptions[i].pressCallback();
			}
			break;
		case button:
			GLFWgamepadstate state;
			if (glfwGetGamepadState(code, &state))
			{
				if (state.buttons[event])
				{
					eventSubscriptions[i].pressCallback();
				}
			}
			break;
		default:
			eventSubscriptions[i].releaseCallback();
			break;
		}
	}

	// joystick logic
	const float joystickThreshold = 0.3f;
	for (int i = 0; i < inputSubscriptions.size(); ++i)
	{
		int code = inputSubscriptions[i].input;
		switch (inputSubscriptions[i].type) {
		case joystick:
		{
			int outputIndex = inputSubscriptions[i].index * 2;
			int count;
			const float* output = glfwGetJoystickAxes(code, &count);

			if (output && count > outputIndex + 1)
			{
				if (abs(output[outputIndex]) > joystickThreshold || abs(output[outputIndex + 1]) > joystickThreshold)
				{
					float adjustedValues[2];
					//TODO scale input
					adjustedValues[0] = (output[outputIndex] - joystickThreshold)/(1.0f - joystickThreshold);
					adjustedValues[1] = (output[outputIndex + 1] - joystickThreshold) / (1.0f - joystickThreshold);
					inputSubscriptions[i].callback(adjustedValues);
				}
			}
		}
		break;
		case trigger:
		{
			int outputIndex = inputSubscriptions[i].index;
			int count;
			const float* output = glfwGetJoystickAxes(code, &count);
			if (output && count > outputIndex)
			{
				inputSubscriptions[i].callback((float*)(output + outputIndex));
			}
		}
		break;
		default:
			break;
		}
	}
}
