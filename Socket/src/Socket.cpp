#include "../include/Socket.h"

#include <strings.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>

Socket::Socket(int socketId): socketId(socketId)
{
    if (socketId == -1)
    {
        std::stringstream message("Failed: socket()\n");
        message << strerror(errno);
        exit(-1);
    }
}

Socket::~Socket()
{    
    if (socketId != -1)
    {
        ::close(socketId);
    }
}

Socket::Socket(Socket&& move)
    : socketId(move.socketId)
{
    move.socketId   = -1;
}

Socket& Socket::operator=(Socket&& move)
{
    std::swap(socketId, move.socketId);
    return *this;
}

bool Socket::SendMessage(std::string const& buffer)
{
    std::size_t size     = buffer.size() + 1;
    std::size_t sentSize = 0;
    while(sentSize != size)
    {
        if(is_connected())
        {
            std::size_t sent = ::write(socketId, buffer.data() + sentSize, size - sentSize);
            if (sent == -1u && errno == EINTR)
            {
                std::cout<<"socket send failed ignorging\n";
                continue;
            }
            if (sent == -1u)
            {
                std::cout<<"socket send failed\n";
                return false;
            }
            if (sent == 0)
            {
                std::cout<<"socket send nothing sent\n";
                return false;
            }
            sentSize += sent;
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool Socket::RecvMessage(std::string& buffer)
{
    buffer.clear();

    char tmp[1024] = {0};
    std::size_t get;
    get= ::read(socketId, tmp, sizeof(tmp));
    buffer.append(tmp, get);
    return true;
}

bool Socket::is_connected()
{
    int error_code;
    unsigned int error_code_size = sizeof(error_code);
    getsockopt(socketId, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
    return (error_code == 0);
}