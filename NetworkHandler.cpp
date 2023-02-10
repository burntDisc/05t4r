#include "NetworkHandler.h"

#include <iostream>

#include <algorithm>
#include <windows.h> // TODO this is for Sleep() (needs to be cross platform)
//****************************************
// TODO should be lock_guard not mutex
//***************************************
using boost::asio::ip::udp;
enum { max_length = 1024 };

NetworkHandler::Gamestate NetworkHandler::localState;
std::queue<NetworkHandler::Gamestate> NetworkHandler::localStates;
std::queue<NetworkHandler::Gamestate> NetworkHandler::remoteStates;
std::mutex NetworkHandler::remoteMutex;
std::mutex NetworkHandler::localMutex;

bool NetworkHandler::running;
std::mutex NetworkHandler::runningMutex;

NetworkHandler::NetworkHandler(int tmp)
{
    Gamestate startState = {
        .position = glm::vec3(10.0f,10.0f,10.0f),
        .orientation = glm::vec3(0.0f,0.0f,1.0f),
        .firingIntensity = 0.0f,
        .firing = false,
        .valid = true
    };
    remoteStates.push(startState);
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

NetworkHandler::Gamestate NetworkHandler::GetRemoteGamestate(double time, Gamestate currentState)
{
    remoteMutex.lock();
    size_t states = remoteStates.size();
    remoteMutex.unlock();

    for (int i = 0; i < states; ++i)
    {
        remoteMutex.lock();
        if (remoteStates.front().time < time)
        {
            remoteStates.pop();
        }
        else
        {
            currentState = remoteStates.front();
            i = states;
        }
        remoteMutex.unlock();
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
    case position:
        localState.position = *(glm::vec3*)value;
        break;
    case orientation:
        localState.orientation = *(glm::vec3*)value;
        break;
    case time:
        localState.time = *(double*)value;
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

            remoteMutex.lock();
            while (remoteStates.size() > 0)
            {
                remoteStates.pop();
            }
            remoteMutex.unlock();

            for (int i = 0; i < reply_length / sizeof(Gamestate); ++i)
            {
                remoteMutex.lock();
                remoteStates.push(in_state[i]);
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

            sock.send_to(boost::asio::buffer(out_state, maxSize * sizeof(Gamestate)), sender_endpoint);

            runningMutex.lock();
        }
        runningMutex.unlock();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}
