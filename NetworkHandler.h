#pragma once

#include <glm/glm.hpp>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>

class NetworkHandler
{
public:

	enum feild { firing, valid, position, orientation };
	// Structure to standardize the vertices used in the meshes
	// should be max 508 bytes for UDP speed safety (no ip fragmentation)
	struct Gamestate
	{
		glm::vec3 position;
		glm::vec3 orientation;
		bool firing;
		bool valid;
	};
	// TODO: without tmp constructor is optomized out O.O
	NetworkHandler(int tmp);
	~NetworkHandler();

	static void SetGamestate(feild feild, void* value);

	static Gamestate GetGamestate(bool consume = false);

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

