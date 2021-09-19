#include "../../Socket/include/Server.h"
#include "../include/MessageReader.h"
#include <iostream>
#include <chrono>
#include <map>

bool eod = false;

void message_sender(Server *server)
{
    MessageReader m;
    m.run();
    while(true)
    {
        message msg = m.getMessage();
        if(msg.eod)
        {
            eod = msg.eod;
            break;
        }
        else
            server->sendToAll(msg);
    }
    std::cout<<"messge sender shutdown\n";
}

void heartbeat(Server *server)
{
    while (!eod)
    {
        server->checkStatusAndDiconnect();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: client <host> <port>\n";
        exit(1);
    }

    static std::string okMsg    = "OK";
    static std::string errorMsg = "ERROR";
    try
    {
        Server  server(argv[1], atoi(argv[2]));
        std::thread ms_thread(message_sender, &server);
        std::thread hb_thread(heartbeat, &server);
        while(!eod)
        {
            server.accept();
        }
        ms_thread.join();
        hb_thread.join();
    }
    catch(std::exception const& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        throw;
    }
}
