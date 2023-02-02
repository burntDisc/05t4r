#include "NetworkHandler.h"

#include <iostream>


#include <windows.h> // TODO this is for Sleep() (needs to be cross platform)
//****************************************
// TODO should be lock_guard not mutex
//***************************************
using boost::asio::ip::udp;
enum { max_length = 1024 };

NetworkHandler::Gamestate NetworkHandler::localState;
NetworkHandler::Gamestate NetworkHandler::remoteState;
std::mutex NetworkHandler::remoteMutex;
std::mutex NetworkHandler::localMutex;

bool NetworkHandler::running;
std::mutex NetworkHandler::runningMutex;

NetworkHandler::NetworkHandler(int tmp)
{
    std::cout << "Starting network threads..." << std::endl;
    server = new std::thread(Server);
    client = new std::thread(Client);
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

NetworkHandler::Gamestate NetworkHandler::GetGamestate(bool consume)
{
    remoteMutex.lock();
    Gamestate current_gamestate = remoteState;
    remoteState.valid = !consume;
    remoteMutex.unlock();
    return current_gamestate;
}

void NetworkHandler::SetGamestate(Gamestate newGamestate)
{
    localMutex.lock();
    localState = newGamestate;
    localMutex.unlock();
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
        while(!sock.send_to(boost::asio::buffer(&initData, request_length), *endpoints.begin()));
       
        runningMutex.lock();
        while (!running)
        {
            runningMutex.unlock();
            Gamestate in_state;
            udp::endpoint sender_endpoint;
            size_t reply_length = sock.receive_from(boost::asio::buffer(&in_state, sizeof(in_state)), sender_endpoint);

            in_state.valid = reply_length > 0;

            remoteMutex.lock();
            remoteState = in_state;
            remoteMutex.unlock();

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
        while (!running)
        {
            runningMutex.unlock();

            localMutex.lock();
            Gamestate out_state = localState;
            localMutex.unlock();

            sock.send_to(boost::asio::buffer(&out_state, sizeof(out_state)), sender_endpoint);

            runningMutex.lock();
        }
        runningMutex.unlock();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}
