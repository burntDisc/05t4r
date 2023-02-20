#include "NetworkHandler.h"

#include <iostream>

#include <cmath>
#include <algorithm>
#include <windows.h> // TODO this is for Sleep() (needs to be cross platform)
//****************************************
// TODO should be lock_guard not mutex
//***************************************
using boost::asio::ip::udp;
enum { max_length = 1024 };

NetworkHandler::Gamestate NetworkHandler::localState;
std::queue<NetworkHandler::Gamestate> NetworkHandler::localStates;
std::vector<NetworkHandler::Gamestate> NetworkHandler::remoteStates;
std::mutex NetworkHandler::remoteMutex;
std::mutex NetworkHandler::localMutex;
std::mutex NetworkHandler::runningMutex;

bool NetworkHandler::running;

NetworkHandler::NetworkHandler(int tmp)
{
    Gamestate startState = {
        .translation = glm::vec3(10.0f,10.0f,10.0f),
        .orientation = glm::vec3(0.0f,0.0f,1.0f),
        .time = 0.0,
        .firingIntensity = 0.0f,
        .firing = false,
        .valid = true
    };
    remoteStates.push_back(startState);
    std::cout << "Starting network threads..." << std::endl;
    running = false;
    client = new std::thread(Client);
    server = new std::thread(Server);
}

NetworkHandler::~NetworkHandler()
{
    runningMutex.lock();
    running = false;
    runningMutex.unlock();
    client->join();
    server->join();
    delete client;
    delete server;
}

void NetworkHandler::PushGamestate(double time)
{
    localState.time = time;
    localMutex.lock();
    localStates.push(localState);
    localMutex.unlock();
}

NetworkHandler::Gamestate NetworkHandler::GetRemoteGamestate(double desiredTime, Gamestate currentState)
{

    double time = desiredTime;// std::min(desiredTime, tmpNetTime);
    double minDiff = 999999999999.9999;
    const int maxStates = 1000;
    const int minStates = 30;

    remoteMutex.lock();
    size_t states = remoteStates.size();
    remoteMutex.unlock();
    if (states > maxStates)
    {
        remoteMutex.lock();
        remoteStates = std::vector<Gamestate>(remoteStates.end() - 100, remoteStates.end());
        remoteMutex.unlock();
        states = maxStates;
    }
    else if (states < minStates)
    {
        time = desiredTime - 0.01;
    }

    for (int i = 0; i < states; ++i)
    {
        remoteMutex.lock();
        Gamestate state = remoteStates[i];
        remoteMutex.unlock();

        double timeDiff = state.time - time;
        if (timeDiff < 0.0)
        {
            remoteMutex.lock();
            remoteStates.erase(remoteStates.begin() + i);
            remoteMutex.unlock();
            --i;
            --states;
        }
        else
        {
            if (timeDiff < minDiff)
            {
                minDiff = timeDiff;
                currentState = GetLerpedState(currentState, state, time);
            }
        }
    }
    return currentState;

}

void NetworkHandler::SetLocalGamestate(feild feild, void* value)
{
    switch (feild) {
    case firingIntensity:
        localState.firingIntensity = *(float*)value;
        break;
    case firing:
        localState.firing = *(bool*)value;
        break;
    case valid:
        localState.valid = *(bool*)value;
        break;
    case translation:
        localState.translation = *(glm::vec3*)value;
        break;
    case orientation:
        localState.orientation = *(glm::vec3*)value;
        break;
    case time:
        localState.time = *(double*)value;
        break;
    case velocity:
        localState.velocity = *(glm::vec3*)value;
        break;
    case colliding:
        localState.colliding = *(bool*)value;
        break;
    default:
        break;
    }
}

void NetworkHandler::Client()
{
    std::cout << "Starting Web Client..." << std::endl;
    try
    {
        std::string host = "192.168.42.108";
        std::string port = "4000";

        boost::asio::io_context io_context;

        udp::socket sock(io_context, udp::endpoint(udp::v4(), 0));

        udp::resolver resolver(io_context);
        udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), host, port);

        // TODO FUTURE: there is where to do hole punching
        char initData[] = "start";
        
        size_t request_length = sizeof(initData);
        bool recieving = false;
        while(!recieving)
        {
            sock.send_to(boost::asio::buffer(&initData, request_length), *endpoints.begin());
            runningMutex.lock();
            recieving = running;
            runningMutex.unlock();
        }

        std::cout << "Connected!" << std::endl;

        runningMutex.lock();
        while (running)
        {
            runningMutex.unlock();
            Gamestate in_state[maxSize];
            udp::endpoint sender_endpoint;
            size_t reply_length = sock.receive_from(boost::asio::buffer(&in_state, maxSize * sizeof(Gamestate)), sender_endpoint);

            for (int i = 0; i < reply_length / sizeof(Gamestate); ++i)
            {
                remoteMutex.lock();
                remoteStates.push_back(in_state[i]);
                remoteMutex.unlock();
            }
            
            runningMutex.lock();
        }
        runningMutex.unlock();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in Client: " << e.what() << "\n";
    }
}

void NetworkHandler::Server()
{
    std::cout << "Starting Web Server..." << std::endl;
    try
    {
        unsigned short port = 4000;

        boost::asio::io_context io_context;

        udp::socket sock(io_context, udp::endpoint(udp::v4(), port));

        // init packet to get client
        char initData[256];
        udp::endpoint sender_endpoint;
        size_t length = sock.receive_from(boost::asio::buffer(initData, 256), sender_endpoint);

        runningMutex.lock();
        running = true;
        while (running)
        {
            runningMutex.unlock();

            Gamestate out_state[maxSize];
            localMutex.lock();
            int statesToSend = std::min(localStates.size(), maxSize);
            localMutex.unlock();

            for (int i = 0; i < statesToSend; ++i)
            {
                localMutex.lock();
                out_state[i] = localStates.front();
                localStates.pop();
                localMutex.unlock();
            }
            if (statesToSend > 0)
            {
                sock.send_to(boost::asio::buffer(out_state, statesToSend * sizeof(Gamestate)), sender_endpoint);
            }

            runningMutex.lock();
        }
        runningMutex.unlock();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}

NetworkHandler::Gamestate NetworkHandler::GetLerpedState(Gamestate oldState, Gamestate newState, double time)
{
    Gamestate lerpedState = newState;

    double timeFactor = (time - oldState.time) / (newState.time - oldState.time);

    lerpedState.translation.x = std::lerp(oldState.translation.x, newState.translation.x, timeFactor);
    lerpedState.translation.y = std::lerp(oldState.translation.y, newState.translation.y, timeFactor);
    lerpedState.translation.z = std::lerp(oldState.translation.z, newState.translation.z, timeFactor);

    lerpedState.orientation.x = std::lerp(oldState.orientation.x, newState.orientation.x, timeFactor);
    lerpedState.orientation.y = std::lerp(oldState.orientation.y, newState.orientation.y, timeFactor);
    lerpedState.orientation.z = std::lerp(oldState.orientation.z, newState.orientation.z, timeFactor);

    lerpedState.velocity.x = std::lerp(oldState.velocity.x, newState.velocity.x, timeFactor);
    lerpedState.velocity.y = std::lerp(oldState.velocity.y, newState.velocity.y, timeFactor);
    lerpedState.velocity.z = std::lerp(oldState.velocity.z, newState.velocity.z, timeFactor);

    return lerpedState;
}
