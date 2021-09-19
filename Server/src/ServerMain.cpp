#include "../../Socket/include/Server.h"
#include "../include/MessageReader.h"
#include <iostream>
#include <chrono>
#include <map>
#include <iostream>
#include <poll.h>

bool eod = false;

void message_sender(Server *server)
{
    MessageReader m;
    m.run();
    while(true)
    {
        message msg;
        bool success = m.getMessage(msg);
        if(success)
        {
            if(msg.eod)
            {
                eod = msg.eod;
                break;
            }
            else
                server->sendToAll(msg);
        }
    }
    std::cout<<"messge sender shutdown\n";
}

void heartbeat_handler(Server *server)
{
    while (server->getClientCount() < 1)
    {
        std::cout<<"No Clients yet\n"; // Wait for at least 1 client to connect
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        continue;
    }
    
    while (!eod)
    {
        for(auto &client : server->getClients())
        {
            if(&client == nullptr)
            {
                std::cout<<"client is null\n";
                continue;
            }
            pollfd pfd = {client.getSockID(), POLLIN, 0};
            int ret = ::poll(&pfd, 1, 300);
            if (ret == -1)
                std::cerr << "Poll Error";
            else if(pfd.revents & POLLIN)
            {
                std::string buf;
                bool res = client.RecvMessage(buf);
                if (res)
                {
                    if(buf == "AreYouThere?")
                    {
                        client.SendMessage("Yes");
                        server->updateHBTracker(client.getSockID(), std::chrono::steady_clock::now());
                    }
                    else
                    {
                        // Ignore other messages
                    }
                }
                else
                {
                    std::cout<<"Unable to read message from client \n";
                    // abort();
                }
            }
        }
    }
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
        std::thread hbH_thread(heartbeat_handler, &server);
        while(!eod)
        {
            server.accept();
        }
        ms_thread.join();
        hb_thread.join();
        hbH_thread.join();
    }
    catch(std::exception const& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        exit(-1);
    }
}
