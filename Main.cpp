#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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
#include "SolidObject.h"
#include "ExplodingObject.h"
#include "InputHandler.h"

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
	glEnable(GL_DEPTH_TEST);	\
	// Uncomment to ignore internals of models for performance
	//glEnable(GL_CULL_FACE);
	// Keeps front faces
	//glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	//glFrontFace(GL_CCW);
	//Configures the blending function
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate Shader objects-------------------------------------------------
	Shader standardShader("standard.vert", "standard.frag");
	Shader explosionShader("explosion.vert", "explosion.geo", "explosion.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");

	// Set Lighting------------------------------------------------------------
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(20.0f, 20.0f, 20.0f);

	standardShader.Activate();
	glUniform4f(glGetUniformLocation(standardShader.ID, "lightColor"),
		lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(standardShader.ID, "lightPos"),
		lightPos.x, lightPos.y, lightPos.z);
	explosionShader.Activate();
	glUniform4f(glGetUniformLocation(explosionShader.ID, "lightColor"),
		lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(explosionShader.ID, "lightPos"),
		lightPos.x, lightPos.y, lightPos.z);

	// Create Game objects ----------------------------------------------------------
	std::vector<GameObject> objects;
	// get current directory

	std::string parentDir = fs::current_path().string();
	// Creates camera object
	Camera camera(window, width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Create SkyBox
	std::string skyboxFacesDirectory = parentDir + "/models/skybox/";
	Skybox skybox(skyboxFacesDirectory);

	// Create ground object
	std::string groundModelPath = parentDir + "/models/ground/scene.gltf";
	glm::vec3 groundTranslation(0.0f, -1.2f, 0.0f);
	glm::quat groundRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 groundScale(10.0f, 2.0f, 10.0f);
	GameObject ground(
		groundModelPath.c_str(),
		groundTranslation,
		groundRotation,
		groundScale
	);

	// Create airplane object
	std::string airplaneModelPath = parentDir + "/models/airplane/scene.gltf";
	glm::vec3 airplaneTranslation(0.0f, -4.0f, -50.0f);
	glm::quat airplaneRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 airplaneScale(5.0f, 5.0f, 5.0f);
	GameObject airplane(
		airplaneModelPath.c_str(),
		airplaneTranslation,
		airplaneRotation,
		airplaneScale
	);

	// Create sword object
	std::string testModelPath = parentDir + "/models/sword/scene.gltf";
	glm::vec3 testTranslation(-50.0f, 40.0f, -20.0f);
	glm::quat testRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 testScale(10.0f, 10.0f, 10.0f);
	GameObject test(
		testModelPath.c_str(),
		testTranslation,
		testRotation,
		testScale
	);

	// Create statue objects
	std::string statueModelPath = parentDir + "/models/statue/scene.gltf";
	glm::vec3 statue0Translation(30.0f, 20.0f, -80.0f);
	glm::quat statue0Rotation = glm::vec3(0.0f, 4.0f, 0.0f);
	glm::vec3 statue0Scale(40.0f, 40.0f, 40.0f);
	SolidObject statueSolid(
		statueModelPath.c_str(),
		statue0Translation,
		statue0Rotation,
		statue0Scale
	);

	glm::vec3 statue1Translation(-20.0f, 20.0f, -80.0f);
	glm::quat statue1Rotation = glm::vec3(0.0f, 4.0f, 0.0f);
	glm::vec3 statue1Scale(40.0f, 40.0f, 40.0f);
	ExplodingObject statueExploding(
		statueModelPath.c_str(),
		statue1Translation,
		statue1Rotation,
		statue1Scale
	);

	//Setup input handler------------------------------------------------------
	InputHandler::SetWindow(window);
	InputHandler::Subscribe(
		InputHandler::joystick,
		GLFW_JOYSTICK_1,
		[&camera](const float* axes) -> void {
			camera.AdjustVelocity(axes);
			camera.AdjustOrientation(axes + 2);
		});
	InputHandler::Subscribe(
		InputHandler::button,
		GLFW_JOYSTICK_1,
		GLFW_GAMEPAD_BUTTON_A,
		[&camera]() -> void {
			camera.TranslateUp();
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
			camera.TranslateUp();
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
	float lastTime = 0.0;
	float time = 0.0;
	float deltaTime;
	unsigned int counter = 0;

	float triggerInterval = 0.1;
	float triggerTime = triggerInterval;
	while (!glfwWindowShouldClose(window))
	{

		// Updates counter and times
		time = glfwGetTime();
		deltaTime = time - lastTime;
		counter++;

		// ############Collison testing area###################################
		if (time > triggerTime) {
			triggerTime = time + triggerInterval;
			glm::vec3 collision = statueSolid.CheckCollison(camera.position);
			if (collision != glm::vec3(0))
			{
				std::cout << "!!!!!!!!!!!!!!Collision!!!!!!!!!!!!!!!!!" << std::endl;
			}
			camera.ProcessCollision(collision);
		}
		// ####################################################################
		// 
		// overwrite fps every loop 1/30 seconds
		if (deltaTime >= 1.0 / 30.0)
		{
			// Creates new title
			std::string FPS = std::to_string((1.0 / deltaTime) * counter);
			std::string ms = std::to_string((deltaTime / counter) * 1000);
			std::string newTitle = "05t4r " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			lastTime = time;
			counter = 0;
		}

		// Specify the color of the background PINK For debug
		// (Skybox Draws over)
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Updates camera matrices
		camera.SetCameraUniforms(standardShader);
		camera.SetCameraUniforms(explosionShader);
		camera.SetSkyboxUniforms(skyboxShader);

		// Draw
		statueSolid.Draw(standardShader);
		statueExploding.Draw(explosionShader);
		airplane.Draw(standardShader);
		test.Draw(standardShader);
		ground.Draw(standardShader);
		skybox.Draw(skyboxShader);

		// Swap back with front buffer
		glfwSwapBuffers(window);

		// Process glfw events
		glfwPollEvents();

		// Handles Inputs and downstream effects
		InputHandler::ProcessInput();
		statueExploding.Set(time);
		camera.Update();
	}

	// Delete and clean up
	standardShader.Delete();
	explosionShader.Delete();
	skyboxShader.Delete();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}