#include "../include/Server.h"
#include <string>
#include <cstdint>
#include <arpa/inet.h>
#include <strings.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>

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
    // struct timeval timeout;      
    // timeout.tv_sec = 1;
    // timeout.tv_usec = 0;

    // if (setsockopt (socketId, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0)
    //     throw std::runtime_error("setsockopt failed RCV");

    // if (setsockopt (socketId, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout) < 0)
    //     throw std::runtime_error("setsockopt failed SND");
}

void Server::accept()
{
    struct  sockaddr_storage    serverStorage;
    socklen_t                   addr_size   = sizeof serverStorage;
    // struct timeval timeout;      
    // timeout.tv_sec = 1;
    // timeout.tv_usec = 0;

    int newSocket = ::accept(socketId, (struct sockaddr*)&serverStorage, &addr_size);
    if (newSocket == -1)
    {
        std::cerr << fprintf(stdout, "%s\n%s\n", "Failed to accept", strerror(errno));
    }
    // if (setsockopt (socketId, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout) < 0)
    //     throw std::runtime_error("setsockopt failed SND");
    
    std::cout<<"Adding new client\n";
    clients.push_back(Socket(newSocket));
}

void Server::sendToAll(message m)
{
    std::cout<<clients.size()<<" Client(s) Connected\n";
    for(auto& client : clients)
    {
        std::cout<<"Sending message "<<m.to_string()<<" to sockID -> "<<client.getSockID()<<"\n";
        bool success = client.SendMessage(m.to_string());
        if(!success)
        {
            std::cout<<"failed\n";
        }
    }
}