// TODO update filesystem include here
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
//------------------------------

#include<math.h>
#include"GameObject.h"
#include"ExplodingObject.h"

const unsigned int width = 1920;
const unsigned int height = 1080;

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

int main()
{
	glfwInit();

	// Set GLFW version to OpenGL 3.3 with Core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window with screen width of monitor
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	// FULL SCREEN DISABLED FOR DEBUGGING
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

	// Generate Shader objects
	Shader standardShader("standard.vert", "standard.frag");
	Shader explosionShader("explosion.vert", "explosion.geo", "explosion.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");

	// Set Lighting
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(20.0f, 20.0f, 20.0f);

	standardShader.Activate();
	glUniform4f(glGetUniformLocation(standardShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(standardShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	explosionShader.Activate();
	glUniform4f(glGetUniformLocation(explosionShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(explosionShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	skyboxShader.Activate();

	//Uniform for texture access format (see shader)
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// Enables the Depth Testing
	glEnable(GL_DEPTH_TEST);

	// Uncomment to ignore internals of models for performance
	//glEnable(GL_CULL_FACE);
	// Keeps front faces
	//glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	//glFrontFace(GL_CCW);
	//Configures the blending function
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Create VertexArrayObject, VertexBufferObject, and ElementBufferObject for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	std::string parentDir = fs::current_path().string();
	// All the faces of the cubemap in order
	std::string skycubeTexturePaths[6] =
	{
		parentDir + "/models/skybox/right.png",
		parentDir + "/models/skybox/left.png",
		parentDir + "/models/skybox/top.png",
		parentDir + "/models/skybox/bottom.png",
		parentDir + "/models/skybox/front.png",
		parentDir + "/models/skybox/back.png",
	};

	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(skycubeTexturePaths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << skycubeTexturePaths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	// Create Game objects ----------------------------------------------------------
	// Create ground object
	std::string groundModelPath = parentDir + "/models/ground/scene.gltf";
	glm::vec3 groundTranslation = glm::vec3(0.0f, -1.2f, 0.0f);
	glm::vec3 groundRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 groundScale = glm::vec3(10.0f, 2.0f, 10.0f);
	GameObject ground(
		groundModelPath.c_str(),
		groundTranslation,
		groundRotation,
		groundScale
	);

	// Create airplane object
	std::string airplaneModelPath = parentDir + "/models/airplane/scene.gltf";
	glm::vec3 airplaneTranslation = glm::vec3(0.0f, -4.0f, -50.0f);
	glm::vec3 airplaneRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 airplaneScale = glm::vec3(5.0f, 5.0f, 5.0f);
	GameObject airplane(
		airplaneModelPath.c_str(),
		airplaneTranslation,
		airplaneRotation,
		airplaneScale
	);

	// Create sword object
	std::string testModelPath = parentDir + "/models/sword/scene.gltf";
	glm::vec3 testTranslation = glm::vec3(-50.0f, 40.0f, -20.0f);
	glm::vec3 testRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 testScale = glm::vec3(10.0f, 10.0f, 10.0f);
	GameObject test(
		testModelPath.c_str(),
		testTranslation,
		testRotation,
		testScale
	);

	// Create statue objects
	std::string statueModelPath = parentDir + "/models/statue/scene.gltf";
	glm::vec3 statue0Translation = glm::vec3(30.0f, 20.0f, -80.0f);
	glm::vec3 statue0Rotation = glm::vec3(0.0f, 4.0f, 0.0f);
	glm::vec3 statue0Scale = glm::vec3(40.0f, 40.0f, 40.0f);
	GameObject statueNormal(
		statueModelPath.c_str(),
		statue0Translation,
		statue0Rotation,
		statue0Scale
	);

	glm::vec3 statue1Translation = glm::vec3(-20.0f, 20.0f, -80.0f);
	glm::vec3 statue1Rotation = glm::vec3(0.0f, 4.0f, 0.0f);
	glm::vec3 statue1Scale = glm::vec3(40.0f, 40.0f, 40.0f);
	ExplodingObject statueExploding(
		statueModelPath.c_str(),
		statue1Translation,
		statue1Rotation,
		statue1Scale
	);

	// Variables to track FPS
	float lastTime = 0.0;
	float time = 0.0;
	float deltaTime;
	unsigned int counter = 0;

	// Main while loop------------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Updates counter and times
		time = glfwGetTime();
		deltaTime = time - lastTime;
		counter++;

		// don't overwrite title every loop
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

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs (Ok with Vsync)
		camera.Inputs(window);

		// Updates and camera matrices
		camera.setCameraUniforms(standardShader);
		camera.setCameraUniforms(explosionShader);

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);


		// Draw statue object
		statueNormal.Draw(standardShader);

		// Draw statue object
		statueExploding.Update(time);
		statueExploding.Draw(explosionShader);

		// Draw airplane object
		airplane.Draw(standardShader);

		// Draw test object
		test.Draw(standardShader);

		// Draw ground object
		ground.Draw(standardShader);

		// Setup and Draw skybox--------------------------
		// set constant depth for skybox
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		// mat4 -> mat3 -> mat4 removes last row and column for translation
		view = glm::mat4(glm::mat3(glm::lookAt(camera.position, camera.position + camera.orientation, camera.up)));
		projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draw skybox last to optimize fragment pass
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);


		// Swap back with front buffer
		glfwSwapBuffers(window);

		// Process glfw events
		glfwPollEvents();
	}

	// Delete and clean up
	standardShader.Delete();
	explosionShader.Delete();
	skyboxShader.Delete();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}