#pragma once

#include <glm/glm.hpp>
#include <thread>
#include <mutex>

class NetworkHandler
{
public:
	// Structure to standardize the vertices used in the meshes
	struct Gamestate
	{
		bool valid;
		glm::vec3 position;
	};
	// TODO: without tmp constructor is optomized out O.O
	NetworkHandler(int tmp);
	~NetworkHandler();

	static void SetGamestate(Gamestate newGamestate);

	static Gamestate GetGamestate();

	static void Client();
	static Gamestate gamestate;
	static std::mutex gamestate_mutex;

	static bool running;
	static std::mutex running_mutex;

private:
	std::thread* client = nullptr;
};

