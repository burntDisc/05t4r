#pragma once

#include <glm/glm.hpp>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include <queue>
class NetworkHandler
{
public:

	enum feild { time, firingIntensity, firing, valid, translation, orientation , velocity, colliding};
	// Structure to standardize the vertices used in the meshes
	// should be max 508 bytes for UDP speed safety (no ip fragmentation)
	struct Gamestate
	{
		glm::vec3 velocity;
		glm::vec3 translation;
		glm::vec3 orientation;
		double time;
		float firingIntensity;
		bool firing;
		bool valid;
		bool colliding;
	};

	//TODO: manage data to prevent cheating
	// TODO: without tmp constructor is optomized out O.O (likely non issue now)
	NetworkHandler(int tmp);
	~NetworkHandler();

	static void SetLocalGamestate(feild feild, void* value);

	static void PushGamestate(double time);
	static Gamestate GetRemoteGamestate(double desiredTime, Gamestate currentState);

	static void Client();
    static void Server();

	static std::vector<Gamestate> remoteStates;
	static std::queue<Gamestate> localStates;
	static Gamestate localState;

	static std::mutex remoteMutex;
	static std::mutex localMutex;
	static std::mutex runningMutex;

	static bool running;

private:
	static Gamestate GetLerpedState(Gamestate oldState, Gamestate newState, double time);
	static const size_t maxSize = 100;
	std::thread* client = nullptr;
    std::thread* server = nullptr;
};

