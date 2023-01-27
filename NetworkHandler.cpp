#include "NetworkHandler.h"

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

NetworkHandler::Gamestate NetworkHandler::gamestate;
std::mutex NetworkHandler::gamestate_mutex;

bool NetworkHandler::running;
std::mutex NetworkHandler::running_mutex;

NetworkHandler::NetworkHandler(int tmp)
{
    std::cout << "running" << std::endl;
    client = new std::thread(Client);
}

NetworkHandler::~NetworkHandler()
{
    running_mutex.lock();
    running = false;
    running_mutex.unlock();
    client->join();
    delete client;
}

NetworkHandler::Gamestate NetworkHandler::GetGamestate()
{
    gamestate_mutex.lock();
    Gamestate current_gamestate = gamestate;
    gamestate_mutex.unlock();
    return current_gamestate;
}

void NetworkHandler::SetGamestate(Gamestate newGamestate)
{
    gamestate_mutex.lock();
    gamestate = newGamestate;
    gamestate_mutex.unlock();
}

void NetworkHandler::Client()
{
    std::cout << "running";
    try
    {
        std::string host = "192.168.42.108";
        std::string port = "4000";

        boost::asio::io_context io_context;

        udp::socket s(io_context, udp::endpoint(udp::v4(), 0));

        udp::resolver resolver(io_context);
        udp::resolver::results_type endpoints =
            resolver.resolve(udp::v4(), host, port);


        gamestate_mutex.lock();
        size_t request_length = sizeof(gamestate);
        s.send_to(boost::asio::buffer(&gamestate, request_length), *endpoints.begin());
        gamestate_mutex.unlock();


        Gamestate defaultState;
        Gamestate* remoteGamestate = &defaultState;

        // TODO should be lock_guard
        running_mutex.lock();

        while (!running)
        {
            running_mutex.unlock();
            udp::endpoint sender_endpoint;
            size_t reply_length = s.receive_from(boost::asio::buffer(remoteGamestate, max_length), sender_endpoint);
            /*
            std::cout << "remote gamestate: " << std::endl;
            std::cout <<remoteGamestate->position.x << std::endl;
            std::cout <<remoteGamestate->position.y << std::endl;
            std::cout <<remoteGamestate->position.z << std::endl;
            */
            gamestate_mutex.lock();
            gamestate.valid = reply_length > 0;
            if (remoteGamestate->position != gamestate.position)
            {
                gamestate = *remoteGamestate;
            }
            gamestate_mutex.unlock();

            running_mutex.lock();
        }
        running_mutex.unlock();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in Client: " << e.what() << "\n";
        running_mutex.unlock();
        gamestate_mutex.unlock();
    }
}
