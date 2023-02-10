#pragma once

#include <glm/glm.hpp>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>


#include <queue>

class NetworkHandler
{
public:

	// Structure to standardize the vertices used in the meshes
	// should be max 508 bytes for UDP speed safety (no ip fragmentation)
	struct Gamestate
	{
		glm::vec3 translation;
		glm::vec3 orientation;
		double time;
		float firingIntensity;
		bool firing;
	};
	// TODO: without tmp constructor is optomized out O.O
	NetworkHandler(int tmp);
	~NetworkHandler();
	static void PushState(double time);

	static Gamestate GetState(double time);

	static void Client();
    static void Server();

	static Gamestate outState;

	static std::mutex outMutex;
	static std::mutex inMutex;

	static bool running;
	static std::mutex runningMutex;

private:
	static const int maxStates = 64;
	static Gamestate inState;
	static std::queue<Gamestate> inStates;
	static std::queue<Gamestate> outStates;
	std::thread* client = nullptr;
    std::thread* server = nullptr;
};

