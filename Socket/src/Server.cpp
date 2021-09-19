#include "../include/Server.h"
#include <string>
#include <cstdint>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <poll.h>

Server::Server(std::string const& hostname, int const port): Socket(::socket(PF_INET, SOCK_STREAM, 0))
{
    struct sockaddr_in serverAddr;
    bzero((char*)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_port         = htons(port);
    serverAddr.sin_addr.s_addr  = inet_addr(hostname.c_str());

    if (::bind(socketId, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) != 0)
    {
        std::stringstream message("Failed: bind()\n");
        message << strerror(errno);
        throw std::runtime_error(message.str());
    }

    if(::listen(socketId, max_queued_requests) != 0)
    {
        std::stringstream message("Failed: listen()\n");
        message << strerror(errno);
        throw std::runtime_error(message.str());
    }
}

void Server::accept()
{
    struct sockaddr_storage serverStorage;
    socklen_t addr_size = sizeof serverStorage;
    

    pollfd pfd = {socketId, POLLIN, 0};
    int ret = ::poll(&pfd, 1, 300);
    if (ret == -1)
        std::cerr << "Poll Error";
    
    else if(pfd.revents & POLLIN)
    {
        int newSocket = ::accept(socketId, (struct sockaddr*)&serverStorage, &addr_size);
        if (newSocket == -1)
        {
            std::cerr << fprintf(stdout, "%s\n%s\n", "Failed to accept", strerror(errno));
        }    
        std::cout<<"Adding new client\n";
        clients.push_back(Socket(newSocket));

        // Timestamp for every new client
        auto pair = std::make_pair(newSocket, std::chrono::steady_clock::now());
        heartbeat_tracker.insert(pair);
    }
}

void Server::sendToAll(message m)
{
    std::cout<<clients.size()<<" Client(s) Connected\n";
    std::vector<Socket>::iterator it = clients.begin();

    while(it != clients.end())
    {
        if(!(it->is_connected()))
        {
            std::cout<<"Client disconnected!!!!\n";
            it = clients.erase(it);
        }
        else
        {
            std::cout<<"Sending message "<<m.to_string()<<" to sockID -> "<<it->getSockID()<<"\n";
            bool success = it->SendMessage(m.to_string());
            if(!success)
            {
                std::cout<<"failed\n";
            }
            ++it;
        }
    }
}

std::vector<Socket>& Server::getClients()
{
    return clients;
}

void Server::checkStatusAndDiconnect()
{
    std::vector<Socket>::iterator it = clients.begin();

    while(it != clients.end())
    {
        auto sockId = it->getSockID();
        auto last_active = heartbeat_tracker[sockId];
        auto now = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_active).count();
        std::cout<<"No Message from "<<sockId<<" since "<<elapsed<<"ms\n";
        if(elapsed > 6000)  // This should come from a configuration file.
        {
            // Disconnect Client
            int error = ::close(socketId);
            std::cout<<"closing coonection "<<sockId<<"\n";
            if(error != 0)
            {
                std::cout<<"Error while disconneting client | error_no = "<<error<<'\n';
            }
            else
            {
                // Remove Client from monitor lists
                std::cout<<"Client disconnected!!!!\n";
                it = clients.erase(it);
                heartbeat_tracker.erase(heartbeat_tracker.find(socketId));
            }
        }
        ++it;
    }
}
