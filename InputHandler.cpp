#include "InputHandler.h"
#include <math.h>

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
				break;
			}
			eventSubscriptions[i].releaseCallback();
			break;
		case mouse:
			if (glfwGetMouseButton(window, code) == event)
			{
				eventSubscriptions[i].pressCallback();
				break;
			}
			eventSubscriptions[i].releaseCallback();
			break;
		case button:
			GLFWgamepadstate state;
			if (glfwGetGamepadState(code, &state))
			{
				if (state.buttons[event])
				{
					eventSubscriptions[i].pressCallback();
					break;
				}
			}
			eventSubscriptions[i].releaseCallback();
			break;
		default:
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
				const float& xAxis = output[outputIndex];
				const float& yAxis = output[outputIndex + 1];
				float squaredMagnitude = xAxis * xAxis + yAxis * yAxis;
				if ( squaredMagnitude > joystickThreshold * joystickThreshold)
				{
					double angle = atan2(xAxis, yAxis);
					double magnitude = sqrt(squaredMagnitude);

					float adjustedValues[2];

					adjustedValues[0] = (magnitude - joystickThreshold) * sin(angle);
					adjustedValues[1] = (magnitude - joystickThreshold) * cos(angle);

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
