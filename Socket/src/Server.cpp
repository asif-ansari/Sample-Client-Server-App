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
        exit(-1);
    }

    if(::listen(socketId, max_queued_requests) != 0)
    {
        std::stringstream message("Failed: listen()\n");
        message << strerror(errno);
        exit(-1);
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
        std::cout<<"Adding new client on socket "<<newSocket<<"\n";
        clients.push_back(Socket(newSocket));
        addToHBTracker(newSocket);
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
            std::cout<<"Client "<<it->getSockID()<<" disconnected!!!!\n";
            removeFromHBTracker(it->getSockID());
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
            // Message Successfully sent, update last_active
            heartbeat_tracker[it->getSockID()] = std::chrono::steady_clock::now();
            ++it;
        }
    }
}

void Server::sendToClient(int fd, std::string msg)
{
    std::cout<<"Send msg to client "<<fd<<" "<<msg<<"\n";
    clients[fd].SendMessage(msg);
}

std::vector<Socket>& Server::getClients()
{
    return clients;
}

void Server::checkStatusAndDiconnect()
{
    std::vector<Socket>::iterator it = clients.begin();
    // std::cout<<"last active "<<heartbeat_tracker[it->getSockID()]<<'\n';
    while(it != clients.end())
    {
        auto last_active = heartbeat_tracker[it->getSockID()];
        auto now = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_active).count();
        std::cout<<"No Message from "<<it->getSockID()<<" since "<<elapsed<<"ms\n";
        if(elapsed > 3000)  // This should come from a configuration file.
        {
            // Disconnect Client
            int error = ::close(it->getSockID());
            std::cout<<"closing coonection "<<it->getSockID()<<" "<<error<<"\n";
            
            // Remove Client from monitor lists
            std::cout<<"Client "<<it->getSockID()<<" disconnected!!!!\n";
            removeFromHBTracker(it->getSockID());
            it = clients.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Server::addToHBTracker(int newSocket)
{
    auto pair = std::make_pair(newSocket, std::chrono::steady_clock::now());
    heartbeat_tracker.insert(pair);
}
void Server::removeFromHBTracker(int sockId)
{
    auto hbt_it = heartbeat_tracker.find(sockId);
    if(hbt_it != heartbeat_tracker.end())
    {
        std::cout<<"Removed from hb tracker "<<sockId<<"\n";
        heartbeat_tracker.erase(hbt_it);
    }
    else
    {
        std::cout<<"NOT Removed from hb tracker "<<sockId<<"\n";
    }
}

void Server::updateHBTracker(int fd, std::chrono::steady_clock::time_point new_time)
{
    if(heartbeat_tracker.find(fd) != heartbeat_tracker.end())
        heartbeat_tracker[fd] = new_time;
    else
        std::cout<<"sockfd not found in hb tracker "<<fd<<'\n';
}
