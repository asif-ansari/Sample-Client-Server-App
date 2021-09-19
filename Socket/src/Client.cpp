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
        throw std::runtime_error(message.str());
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
            throw std::runtime_error(message.str());
        }
    }
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
            throw std::runtime_error(message.str());
        }
    }
    return result;
}