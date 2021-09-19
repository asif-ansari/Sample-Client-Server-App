#include "../include/Client.h"
#include <sstream>
#include <cstring>


Client::Client(std::string const& hostname, int const port): Socket(::socket(PF_INET, SOCK_STREAM, 0))
{
    struct sockaddr_in serverAddr;
    socklen_t addrSize = sizeof(serverAddr);
    bzero((char*)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_port         = htons(port);
    serverAddr.sin_addr.s_addr  = inet_addr(hostname.c_str());

    if (::connect(socketId, (struct sockaddr*)&serverAddr, addrSize) != 0)
    {
        std::stringstream message("Failed: connect()\n");
        message << strerror(errno);
        exit(-1);
    }
    else
    {
        //Connected successfully
        last_active = std::chrono::steady_clock::now();
    }
}

bool Client::SendMessage(std::string const& buffer)
{
    bool result = Socket::SendMessage(buffer);
    if (result)
    {
        if (::shutdown(socketId, SHUT_WR) != 0)
        {
            std::stringstream message("Failed: shutdown()\n");
            message << strerror(errno);
            exit(-1);
        }
    }
    // Message sent successfully
    last_active = std::chrono::steady_clock::now();
    return result;
}

bool Client::RecvMessage(std::string & buffer)
{
    bool result = Socket::RecvMessage(buffer);
    if (result)
    {
        if (::shutdown(socketId, SHUT_WR) != 0)
        {
            std::stringstream message("Failed: shutdown()\n");
            message << strerror(errno);
            exit(-1);
        }
    }
    // Message recived
    last_active = std::chrono::steady_clock::now();
    return result;
}

std::chrono::steady_clock::time_point& Client::getLastActive()
{
    return last_active;
}

void Client::setLastActive(std::chrono::steady_clock::time_point new_time)
{
    last_active = new_time;
}
