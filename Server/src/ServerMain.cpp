#include "../../Socket/include/Server.h"
#include "../include/MessageReader.h"
#include <iostream>
#include <sstream>
#include <cstdint>

void message_sender(Server *server)
{
    MessageReader m;
    m.run();
    while (true)
    {
        message msg = m.getMessage();
        server->sendToAll(msg);
    }
    std::cout<<"messge sender shutdown\n";
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
        std::thread t(message_sender, &server);
        while(true)
        {
            server.accept();
        }
        std::cout<<"Shutting down\n";
        t.join();
        std::cout<<"Shutting down 2\n";
    }
    catch(std::exception const& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        throw;
    }
    std::cout<<"Shutting down end\n";
}
