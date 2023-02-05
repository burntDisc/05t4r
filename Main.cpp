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

#include "Shader.h"
#include "GameObject.h"
#include "Camera.h"
#include "Skybox.h"
#include "InputHandler.h"
#include "MotionHandler.h"
#include "GeneratedGround.h"
#include "GeneratedWalls.h"
#include "ProjectileStream.h"
#include "NetworkHandler.h"
#include "Opponent.h"

int main()
{
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
	// FULL SCREEN DISABLED FOR DEBUGGING
	unsigned int width = 1500;
	unsigned int height = 900;
	GLFWwindow* window = glfwCreateWindow(1500, 900, "5t4r", NULL, NULL);
	// GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "5t4r", monitor, NULL);
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
	// Uncomment to ignore internals of models (breaks explosion internals)
	//glEnable(GL_CULL_FACE);
	// Keeps front faces
	//glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	//glFrontFace(GL_CCW);
	//Configures the blending function
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate Shader objects-------------------------------------------------
	std::vector<Shader> shaders;
	Shader standardShader("standard.vert", "standard.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");
	shaders.push_back(standardShader);
	shaders.push_back(skyboxShader);

	// Set Lighting------------------------------------------------------------
	glm::vec4 lightColor = glm::vec4(1.2f, 1.2f, 1.2f, 1.2f);
	glm::vec3 lightPos = glm::vec3(20.0f, 20.0f, 20.0f);

	for (int i = 0; i < shaders.size(); ++i)
	{
		Shader& shader = shaders[i];
		if (shader.ID != skyboxShader.ID)
		{
			shader.Activate();
			glUniform4f(glGetUniformLocation(shader.ID, "lightColor"),
				lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shader.ID, "lightPos"),
				lightPos.x, lightPos.y, lightPos.z);
		}
	}

	// Create Game objects ----------------------------------------------------------
	// get current directory
	std::string parentDir = fs::current_path().string();

	// Create projectile object
	std::string projectilePath = parentDir + "/models/projectile/scene.gltf";
	glm::vec3 projectileScale(2.0f, 2.0f, 2.0f);
	glm::vec3 projectileTranslationAdjustment(0.0f, 0.0f, 0.0f);
	ProjectileStream badProjectiles(
		projectilePath.c_str(),
		projectileScale
	);

	// Create projectile object
	ProjectileStream goodProjectiles(
		projectilePath.c_str(),
		projectileScale
	);

	// Create Opponent object
	std::string oppModelPath = parentDir + "/models/statue/scene.gltf";
	glm::vec3 oppTranslation(0.0f, 50.0f, 50.0f);
	glm::quat oppRotation = glm::vec3(0.0f, -acos(0), 0.0f);
	glm::vec3 oppScale(10.0f, 10.0f, 10.0f);
	Opponent opp(
		oppModelPath.c_str(),
		badProjectiles,
		oppTranslation,
		oppScale,
		oppRotation
	);
	// Creates camera object
	Camera camera(window, width, height, glm::vec3(10.0f, 10.0f, 2.0f), &opp);

	// Create SkyBox
	std::string skyboxFacesDirectory = parentDir + "/models/skybox/";
	Skybox skybox(skyboxFacesDirectory);

	/*
	// Create statue object
	std::string statueModelPath = parentDir + "/models/statue/scene.gltf";
	glm::vec3 statueTranslation(40.0f, 40.0f, 120.0f);
	glm::quat statueRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 statueScale(10.0f, 10.0f, 10.0f);
	GameObject statue(
		statueModelPath.c_str(),
		statueTranslation,
		statueScale,
		statueRotation
	);
	*/

	// TODO pass by reference in GameObject contructors
	
	// Create wall object
	std::string wallModelPath = parentDir + "/models/test0/scene.gltf";
	glm::vec3 wallTranslation(0.0f, 0.0f, 0.1f);
	glm::quat wallRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 wallScale(1.0f, 1.0f, 1.0f);
	GeneratedWalls wall(
		&camera,
		wallModelPath.c_str(),
		wallTranslation,
		wallScale,
		wallRotation
	);

	MotionHandler::AddSolidObject(&wall);

	// Create floor object
	std::string floorPath = parentDir + "/models/ground/scene.gltf";
	glm::vec3 floorTranslation(0.0f, 0.0f, 0.0f);
	glm::quat floorRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 floorScale(10.0f, 10.0f, 10.0f);
	GeneratedGround floor(
		&camera,
		floorPath.c_str(),
		floorTranslation,
		floorScale,
		floorRotation
	);

	MotionHandler::AddSolidObject(&floor);

	//Setup input handler------------------------------------------------------
	InputHandler::SetWindow(window);
	InputHandler::Subscribe(
		InputHandler::trigger,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
		[&camera](float* input) -> void {
			camera.ZoomAndLock(input);
		});
	InputHandler::Subscribe(
		InputHandler::trigger,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
		[&goodProjectiles, &camera](float* input) -> void {
			goodProjectiles.Fire(
				camera.translation,
				camera.orientation,
				input);
		});
	InputHandler::Subscribe(
		InputHandler::joystick,
		GLFW_JOYSTICK_1,
		0,
		[&camera](float* input) -> void {
			camera.AdjustVelocity(input);
		});
	InputHandler::Subscribe(
		InputHandler::joystick,
		GLFW_JOYSTICK_1,
		1,
		[&camera](float* input) -> void {
			camera.AdjustOrientation(input);
		});
	InputHandler::Subscribe(
		InputHandler::button,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_BUTTON_A,
		[&camera]() -> void {
			camera.Jump();
		});
	InputHandler::Subscribe(
		InputHandler::button,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
		[&camera]() -> void {
			camera.DashLeft();
		},
		[&camera]() -> void {
			camera.ReadyDashLeft();
		});
	InputHandler::Subscribe(
		InputHandler::button,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
		[&camera]() -> void {
			camera.DashRight();
		},
		[&camera]() -> void {
			camera.ReadyDashRight();
		});
	InputHandler::Subscribe(
		InputHandler::button,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_BUTTON_Y,
		[&camera]() -> void {
			camera.DashForward();
		},
		[&camera]() -> void {
			camera.ReadyDashForward();
		});
	InputHandler::Subscribe(
		InputHandler::button,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_BUTTON_X,
		[&camera]() -> void {
			camera.DashBack();
		},
		[&camera]() -> void {
			camera.ReadyDashBack();
		});
	InputHandler::Subscribe(
		InputHandler::button,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_BUTTON_B,
		[&camera]() -> void {
			camera.Break();
		});
	//Connect to network
	NetworkHandler NH(0);

	// Main Render loop--------------------------------------------------------
	// 
	// Variables to track FPS
	double lastTime = 0.0;
	double time = 0.0;
	double deltaTime;
	unsigned int counter = 0;
	unsigned int lastCycle = 0;

	double GameLoopInterval = 1.00/30.00;  // seconds
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
			std::string FPS = std::to_string((1.0 / deltaTime) * counter);
			std::string ms = std::to_string((deltaTime / counter) * 1000);
			std::string xO = std::to_string(camera.orientation.x);
			std::string yO = std::to_string(camera.orientation.y);
			std::string zO = std::to_string(camera.orientation.z);
			std::string xP = std::to_string(camera.translation.x);
			std::string yP = std::to_string(camera.translation.y);
			std::string zP = std::to_string(camera.translation.z);
			std::string newTitle = "05t4r " + FPS + "FPS / " + ms + "ms Orientation: x: " + xO + " y: " + yO + " z: " + zO + "  Position: x: " + xP + " y: " + yP + " z: " + zP;
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			lastTime = time;
			counter = 0;
			// Handles Inputs and downstream effects
			InputHandler::ProcessInput();
			floor.Update();
			camera.Update((float)time);
			badProjectiles.Update((float)time);
			goodProjectiles.Update((float)time);
			opp.Update((float)time);
			//projectile.Update();
			if (lastCycle == counter)
			{
				std::cout << "-CPU OVERLOAD-" << std::endl;
			}
			lastCycle = counter + 1;
		}

		// Specify the color of the background GREEN For debug
		// (Skybox Draws over)
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Updates shader camera matrices
		for (int i = 0; i < shaders.size(); ++i)
		{
			Shader& shader = shaders[i];
			if (shader.ID == skyboxShader.ID)
			{
				camera.SetSkyboxUniforms(shader);
			}
			else
			{
				camera.SetCameraUniforms(shader);
			}
		}

		// Draw
		goodProjectiles.Draw(standardShader);
		badProjectiles.Draw(standardShader);
		floor.Draw(standardShader);
		wall.Draw(standardShader);
		skybox.Draw(skyboxShader);
		opp.Draw(standardShader);
		//statue.Draw(standardShader);


		// Swap back with front buffer
		glfwSwapBuffers(window);

		// Process glfw events
		glfwPollEvents();
	}

	// Delete and clean up
	for (int i = 0; i < shaders.size(); ++i) 
	{
		shaders[i].Delete();

	}
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}