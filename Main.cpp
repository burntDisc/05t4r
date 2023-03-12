#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <string>
// TODO update filesystem include here----------------------
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
//----------------------------------------------------------

#include "Gameplay.h"
#include "Shader.h"
#include "GameObject.h"
#include "Player.h"
#include "Skybox.h"
#include "InputHandler.h"
#include "MotionHandler.h"
#include "GeneratedGround.h"
#include "GeneratedWalls.h"
#include "ProjectileStream.h"
#include "NetworkHandler.h"
#include "Opponent.h"
#include "Overlay.h"
#include "EnergyBarOverlay.h"
#include "HealthBarOverlay.h"
#include "ReticleOverlay.h"
#include "Camera.h"
#include "Rig.h"
#include "AudioHandler.h"
#include "Title.h"

int main()
{
	// get current directory

	// start audio
	Audio audio = Audio();

	Audio::PlayTheme(mainTheme);

	// Set up window-----------------------------------------------------------
	// initilize glfw to handle input and window
	glfwInit();
	// Set GLFW version to OpenGL 3.3 with Core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window with screen width of monitor (DISABLED)
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	// Set Screen and Size
	int width = 1500; //mode->width;//1500;
	int height = 900; //mode->height;//900;
	GLFWwindow* window = glfwCreateWindow(width, height, "5t4r", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "5t4r", monitor, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//Load GLAD to configure opengl---------------------------------------------
	gladLoadGL();
	glViewport(0, 0, width, height);
	// Enables the Depth Testing
	glEnable(GL_DEPTH_TEST);
	//Configures the blending function
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Uncomment to ignore internals of models (breaks explosion internals)
	//glEnable(GL_CULL_FACE);
	// Keeps front faces
	//glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	//glFrontFace(GL_CCW);
	// Loading Overlay Logic

	// Preload Title Screen---------------------------------------------------
	Title loadingScreen = Title();
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreen.Draw();
	glfwSwapBuffers(window);

	// Load Game Scene
	Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
	Gameplay gameplay(width, height);

	// Main Render loop--------------------------------------------------------
	// 
	// Variables to track FPS
	double lastTime = 0.0;
	double time = 0.0;
	double deltaTime;
	unsigned int counter = 0;
	unsigned int lastCycle = 0;

	int hits = 0;
	double GameLoopInterval = 1.00/30.00;  // seconds
	std::deque<double> framerates;
	int sampleFrames = 100;

	while (!glfwWindowShouldClose(window))
	{

		// Updates counter and times
		time = glfwGetTime();
		deltaTime = time - lastTime;
		counter++;

		// ####################################################################
		// 
		// Creating Constant gamespeed at normal operating load
		if (deltaTime >= GameLoopInterval)
		{
			// Creates new title
			double fps = (1.0 / deltaTime) * counter;
			framerates.push_front(fps);
			double sum = 0.0;
			for (int i = 0; i < framerates.size(); ++i)
			{
				sum += framerates[i];
			}
			double avgFps = sum / framerates.size();
			if (framerates.size() > sampleFrames) {
				framerates.pop_back();
			}
			std::string sampleSize = std::to_string(sampleFrames);
			std::string FPS = std::to_string(avgFps);
			std::string newTitle = "-------Avg FPS(" + sampleSize + "frames): " + FPS;
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			lastTime = time;
			counter = 0;
			gameplay.Update(time);
			InputHandler::UpdateGamepad();

			if (lastCycle == counter)
			{
				std::cout << "-CPU OVERLOAD-" << std::endl;
			}
			lastCycle = counter + 1;
		}

		// Specify the color of the background GREEN For debug
		// (Skybox Draws over)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw
		gameplay.Draw();


		// Swap back with front buffer
		glfwSwapBuffers(window);

		// Process glfw events
		glfwPollEvents();
	}

	// Delete and clean up
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}