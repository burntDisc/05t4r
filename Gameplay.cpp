#include "Gameplay.h"

#include <glm/glm.hpp>
#include <string>

#include "ProjectileStream.h"
#include "Opponent.h"
#include "Player.h"
#include "HealthBarOverlay.h"
#include "EnergyBarOverlay.h"
#include "ReticleOverlay.h"
#include "Skybox.h"
#include "GeneratedWalls.h"
#include "GeneratedGround.h"
#include "MotionHandler.h"
#include "Camera.h"

// TODO update filesystem include here----------------------
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
//----------------------------------------------------------

Gameplay::Gameplay(float width, float height) : 
	camera(width, height),
	networkHandler("192.168.42.108")
{
	std::string parentDir = fs::current_path().string();
	// Create Shaders
	Shader HUDShader("shaders/2D.vert", "shaders/HUD.frag");
	Shader standardShader("shaders/standard.vert", "shaders/standard.frag");
	Shader explosionShader("shaders/explosion.vert", "shaders/explosion.geo", "shaders/explosion.frag");
	Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
	shaders.push_back(HUDShader);
	shaders.push_back(standardShader);
	shaders.push_back(explosionShader);
	shaders.push_back(skyboxShader);

	// Set Lighting------------------------------------------------------------ TODO: not accurate
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
	// Create Game objects --------------------------------------------------------------

	// Create Projectiles------------------------------------------------------------
	glm::vec3 projectileScale(0.5f, 0.5f, 0.5f);
	glm::vec3 projectileOrientation(0.0f, 0.0f, -1.0f);
	glm::vec3 projectileTranslationAdjustment(0.0f, 0.0f, 0.0f);

	// Create opponent projectile stream-----------------------------------------
	std::string badProjectilePath = parentDir + "/models/badProjectile/scene.gltf";

	ProjectileStream* badProjectiles = new ProjectileStream(
		explosionShader,
		standardShader,
		badProjectilePath.c_str(),
		projectileScale,
		projectileOrientation
	);
	updatables.push_back(badProjectiles);
	drawables.push_back(badProjectiles);

	// Create player projectile object-------------------------------------------
	std::string goodProjectilePath = parentDir + "/models/goodProjectile/scene.gltf";
	ProjectileStream* goodProjectiles = new ProjectileStream(
		explosionShader,
		standardShader,
		goodProjectilePath.c_str(),
		projectileScale,
		projectileOrientation,
		goodProjectilePath.c_str()
	);
	updatables.push_back(goodProjectiles);
	drawables.push_back(goodProjectiles);

	// Create Opponent object---------------------------------------------------------
	std::string oppModelPath = parentDir + "/models/segment/scene.gltf";
	glm::vec3 oppTranslation(0.0f, 0.0f, 0.0f);
	glm::quat oppRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 oppScale(1.0f, 1.0f, 1.0f);
	Opponent* opp = new Opponent(
		standardShader,
		oppModelPath.c_str(),
		*badProjectiles,
		oppTranslation,
		oppScale,
		oppRotation
	);
	updatables.push_back(opp);
	drawables.push_back(opp);

	// Creates player object----------------------------------------------------------
	Player* player = new Player(
		glm::vec3(3.0f, 10.0f, 40.0f),
		*opp,
		*goodProjectiles);

	updatables.push_back(player);


	drawables.push_back(opp);
	// Create wall object-------------------------------------------------------------
	std::string wallModelPath = parentDir + "/models/wall/scene.gltf";
	glm::vec3 wallTranslation(0.0f, 0.0f, 0.1f);
	glm::quat wallRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 wallScale(1.0f, 1.0f, 1.0f);
	GeneratedWalls* wall = new GeneratedWalls(
		standardShader,
		*player,
		wallModelPath.c_str(),
		wallTranslation,
		wallScale,
		wallRotation
	);
	MotionHandler::AddSolidObject(wall);
	updatables.push_back(wall);
	drawables.push_back(wall);

	// Create floor object-----------------------------------------------------------
	std::string floorPath = parentDir + "/models/ground/scene.gltf";
	glm::vec3 floorTranslation(0.0f, 0.0f, 0.0f);
	glm::quat floorRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 floorScale(10.0f, 10.0f, 10.0f);
	GeneratedGround* floor = new GeneratedGround(
		standardShader,
		*player,
		floorPath.c_str(),
		floorTranslation,
		floorScale,
		floorRotation
	);
	MotionHandler::AddSolidObject(floor);
	updatables.push_back(floor);
	drawables.push_back(floor);

	// Create SkyBox----------------------------------------------------------------------
	std::string skyboxFacesDirectory = parentDir + "/models/skybox/";
	Skybox* skybox = new Skybox(skyboxShader, skyboxFacesDirectory);
	drawables.push_back(skybox);

	// Create HUD---------------------------------------------------------------------
	HealthBarOverlay* healthBar = new HealthBarOverlay(HUDShader, *player);
	updatables.push_back(healthBar);
	drawables.push_back(healthBar);
	EnergyBarOverlay* energyBar = new EnergyBarOverlay(HUDShader, *player);
	updatables.push_back(energyBar);
	drawables.push_back(energyBar);
	ReticleOverlay* reticle = new ReticleOverlay(HUDShader, *player, width, height);
	updatables.push_back(reticle);
	drawables.push_back(reticle);

	// Camera
	camera.Bind(& player->translation, & player->orientation, & player->feildOfView);
}

void Gameplay::Update(double time)
{

	// Updates shader camera matrices
	for (int i = 0; i < shaders.size(); ++i)
	{
		Shader& shader = shaders[i];

		//skybox shader index
		if (i == shaders.size()  - 1)
		{
			camera.SetSkyboxUniforms(shader);
		}
		else
		{
			camera.SetCameraUniforms(shader);
		}
	}

	Scene::Update(time);
}