#include "NetworkHandler.h"

#include <iostream>
#include <algorithm>

#include <windows.h> // TODO this is for Sleep() (needs to be cross platform)
//****************************************
// TODO should be lock_guard not mutex
//***************************************
using boost::asio::ip::udp;
enum { max_length = 1024 };

std::mutex NetworkHandler::inMutex;
std::mutex NetworkHandler::outMutex;
bool NetworkHandler::running;
std::mutex NetworkHandler::runningMutex;
std::queue<NetworkHandler::Gamestate> NetworkHandler::inStates;
std::queue<NetworkHandler::Gamestate> NetworkHandler::outStates;
NetworkHandler::Gamestate NetworkHandler::outState;
NetworkHandler::Gamestate NetworkHandler::inState;

NetworkHandler::NetworkHandler(int tmp)
{
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

void NetworkHandler::PushState(double time)
{
    outState.time = time;
    outMutex.lock();
    outStates.push(outState);
    outMutex.unlock();
}

NetworkHandler::Gamestate NetworkHandler::GetState(double time)
{
    inMutex.lock();
    while (!inStates.empty())
    {
        if (inStates.front().time < time)
        {
            inStates.pop();
        }
        else
        {
            inState = inStates.front();
        }
    }
    inMutex.unlock();

    return inState;
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

            Gamestate recievedStates[maxStates];
            udp::endpoint sender_endpoint;
            size_t replyLength = sock.receive_from(boost::asio::buffer(recievedStates, maxStates * sizeof(Gamestate)), sender_endpoint);

            for (int i = 0; i < replyLength / sizeof(Gamestate); ++i)
            {
                inMutex.lock();
                inStates.push(recievedStates[i]);
                inMutex.unlock();
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

            Gamestate toSendStates[maxStates];

            outMutex.lock();
            size_t numStates = std::min(outStates.size(), maxStates);
            outMutex.unlock();

            for (int i = 0; i < numStates; ++i)
            {
                outMutex.lock();
                toSendStates[i] = outStates.front();
                outStates.pop();
                outMutex.unlock();
            }

            sock.send_to(boost::asio::buffer(toSendStates, numStates * sizeof(Gamestate)), sender_endpoint);

            runningMutex.lock();
        }
        runningMutex.unlock();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}
