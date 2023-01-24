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

int main()
{
	// Set up OpenGl stack and window------------------------------------------
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
	//Load GLAD to configure opengl
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
	std::vector<GameObject> objects;
	// get current directory

	std::string parentDir = fs::current_path().string();
	// Creates camera object
	Camera camera(window, width, height, glm::vec3(10.0f, 10.0f, 2.0f));

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

	// Create projectile object
	std::string projectilePath = parentDir + "/models/projectile/scene.gltf";
	glm::vec3 projectileScale(2.0f, 2.0f, 2.0f);
	glm::vec3 projectileTranslationAdjustment(0.0f, 0.0f, 0.0f);
	ProjectileStream projectile(
		projectilePath.c_str(),
		projectileScale
	);

	//Setup input handler------------------------------------------------------
	InputHandler::SetWindow(window);
	InputHandler::Subscribe(
		InputHandler::button,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_BUTTON_X,
		[&projectile, &camera]() -> void {
			std::cout << "fired" << std::endl;
			projectile.Fire(
				camera.position,
				camera.orientation);
		});
	InputHandler::Subscribe(
		InputHandler::keyboard,
		GLFW_KEY_Q,
		GLFW_PRESS,
		[&projectile, &camera]() -> void {
			std::cout << "fired" << std::endl;
			projectile.Fire(
				camera.position,
				camera.orientation);
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
		GLFW_GAMEPAD_BUTTON_B,
		[&camera]() -> void {
			camera.Boost();
		});
	InputHandler::Subscribe(
		InputHandler::keyboard,
		GLFW_KEY_W,
		GLFW_PRESS,
		[&camera]() -> void {
			camera.Forward();
		});
	InputHandler::Subscribe(
		InputHandler::keyboard,
		GLFW_KEY_LEFT_SHIFT,
		GLFW_PRESS,
		[&camera]() -> void {
			camera.Boost();
		});
	InputHandler::Subscribe(
		InputHandler::keyboard,
		GLFW_KEY_A,
		GLFW_PRESS,
		[&camera]() -> void {
			camera.TranslateLeft();
		});
	InputHandler::Subscribe(
		InputHandler::keyboard,
		GLFW_KEY_S,
		GLFW_PRESS,
		[&camera]() -> void {
			camera.Back();
		});
	InputHandler::Subscribe(
		InputHandler::keyboard,
		GLFW_KEY_D,
		GLFW_PRESS,
		[&camera]() -> void {
			camera.TranslateRight();
		});
	InputHandler::Subscribe(
		InputHandler::keyboard,
		GLFW_KEY_SPACE,
		GLFW_PRESS,
		[&camera]() -> void {
			camera.Jump();
		});
	InputHandler::Subscribe(
		InputHandler::mouse,
		GLFW_MOUSE_BUTTON_LEFT,
		GLFW_PRESS,
		[&camera, window]() -> void {
			camera.BindCursor();
		});
	InputHandler::Subscribe(
		InputHandler::mouse,
		GLFW_MOUSE_BUTTON_LEFT,
		GLFW_RELEASE,
		[&camera, window]() -> void {
			camera.UnbindCursor();
		});

	// Main Render loop--------------------------------------------------------

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
			std::string xP = std::to_string(camera.position.x);
			std::string yP = std::to_string(camera.position.y);
			std::string zP = std::to_string(camera.position.z);
			std::string newTitle = "05t4r " + FPS + "FPS / " + ms + "ms Orientation: x: " + xO + " y: " + yO + " z: " + zO + "  Position: x: " + xP + " y: " + yP + " z: " + zP;
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			lastTime = time;
			counter = 0;
			// Handles Inputs and downstream effects
			InputHandler::ProcessInput();
			floor.Update();
			camera.Update((float)time);
			projectile.Update((float)time);
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
		projectile.Draw(standardShader);
		floor.Draw(standardShader);
		wall.Draw(standardShader);
		skybox.Draw(skyboxShader);
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