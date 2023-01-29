#pragma once

#include <glm/glm.hpp>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>

class NetworkHandler
{
public:
	// Structure to standardize the vertices used in the meshes
	struct Gamestate
	{
		bool valid;
		glm::vec3 position;
		glm::vec3 orientation;
	};
	// TODO: without tmp constructor is optomized out O.O
	NetworkHandler(int tmp);
	~NetworkHandler();

	static void SetGamestate(Gamestate newGamestate);

	static Gamestate GetGamestate();

	static void Client();
    static void Server();

	static Gamestate remoteState;
	static Gamestate localState;

	static std::mutex remoteMutex;
	static std::mutex localMutex;

	static bool running;
	static std::mutex runningMutex;

private:
	std::thread* client = nullptr;
    std::thread* server = nullptr;
};

