#pragma once

#include <string>
#include <arpa/inet.h>

class Socket
{
    protected:
        int socketId;
    public:
        Socket(int socketId);
        ~Socket();
        Socket(Socket&&);
        Socket& operator=(Socket&&);
        Socket(Socket const&)               = delete;
        Socket& operator=(Socket const&)    = delete;

        virtual bool SendMessage(std::string const& buffer);
        virtual bool RecvMessage(std::string& buffer);
        int getSockID() { return socketId; }
        bool is_connected();
};

