#pragma once

#include <glm/glm.hpp>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include <queue>
class NetworkHandler
{
public:

	enum feild { time, firingIntensity, firing, valid, position, orientation };
	// Structure to standardize the vertices used in the meshes
	// should be max 508 bytes for UDP speed safety (no ip fragmentation)
	struct Gamestate
	{
		glm::vec3 position;
		glm::vec3 orientation;
		double time;
		float firingIntensity;
		bool firing;
		bool valid;
	};
	// TODO: without tmp constructor is optomized out O.O
	NetworkHandler(int tmp);
	~NetworkHandler();

	static void SetLocalGamestate(feild feild, void* value);

	static Gamestate GetRemoteGamestate(double time);

	static void Client();
    static void Server();

	static std::queue<Gamestate> remoteState;
	static Gamestate localState;

	static std::mutex remoteMutex;
	static std::mutex localMutex;

	static bool running;
	static std::mutex runningMutex;

private:
	static const size_t maxSize = 10;
	std::thread* client = nullptr;
    std::thread* server = nullptr;
};

