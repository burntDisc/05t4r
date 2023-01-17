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
#include "ExplodingObject.h"
#include "InputHandler.h"
#include "MotionHandler.h"
#include "GeneratedGround.h"

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
	Camera camera(window, width, height, glm::vec3(10.0f, 10.0f, 2.0f));

	// Create SkyBox
	std::string skyboxFacesDirectory = parentDir + "/models/skybox/";
	Skybox skybox(skyboxFacesDirectory);

	// Create wall object
	std::string wallModelPath = parentDir + "/models/test0/scene.gltf";
	glm::vec3 wallTranslation(-1.0f, 0.0f, -90000.0f);
	glm::quat wallRotation = glm::vec3(0.0f, 0.0f, -3.14159 / 2.0);
	glm::vec3 wallScale(1.0f, 1.0f, 1.0f);
	GameObject wall(
		wallModelPath.c_str(),
		wallTranslation,
		wallRotation,
		wallScale
	);

	// Create statue object
	std::string statueModelPath = parentDir + "/models/statue/scene.gltf";
	glm::vec3 statue1Translation(-20.0f, 15.0f, -900080.0f);
	glm::quat statue1Rotation = glm::vec3(0.0f, 4.0f, 0.0f);
	glm::vec3 statue1Scale(40.0f, 40.0f, 40.0f);
	ExplodingObject statueExploding(
		statueModelPath.c_str(),
		statue1Translation,
		statue1Rotation,
		statue1Scale
	);

	// Create floor object
	std::string floorPath = parentDir + "/models/ground/scene.gltf";
	glm::vec3 floorTranslation(0.0f, 0.0f, 0.0f);
	glm::quat floorRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 floorScale(10.0f, 10.0f, 10.0f);
	GeneratedGround floor(
		&camera,
		floorPath.c_str(),
		floorTranslation,
		floorRotation,
		floorScale
	);

	MotionHandler::AddSolidObject(&floor);

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
	double lastTime = 0.0;
	double time = 0.0;
	double deltaTime;
	unsigned int counter = 0;

	while (!glfwWindowShouldClose(window))
	{

		// Updates counter and times
		time = glfwGetTime();
		deltaTime = time - lastTime;
		counter++;

		// ####################################################################
		// 
		// overwrite fps every loop 1/30 seconds
		if (deltaTime >= 1.0 / 35.0)
		{
			// Creates new title
			std::string FPS = std::to_string((1.0 / deltaTime) * counter);
			std::string ms = std::to_string((deltaTime / counter) * 1000);
			std::string x = std::to_string(camera.position.x);
			std::string y = std::to_string(camera.position.y);
			std::string z = std::to_string(camera.position.z);
			std::string newTitle = "05t4r " + FPS + "FPS / " + ms + "ms  x: " + x + " y: " + y + " z: " + z;
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			lastTime = time;
			counter = 0;
			InputHandler::ProcessInput();
			camera.Update();
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
		statueExploding.Draw(explosionShader);
		wall.Draw(standardShader);
		floor.Draw(standardShader);
		skybox.Draw(skyboxShader);

		// Swap back with front buffer
		glfwSwapBuffers(window);

		// Process glfw events
		glfwPollEvents();

		// Handles Inputs and downstream effects
		statueExploding.Update(time);
		floor.UpdatePanels();
	}

	// Delete and clean up
	standardShader.Delete();
	explosionShader.Delete();
	skyboxShader.Delete();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}