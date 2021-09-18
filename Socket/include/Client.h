#include "Socket.h"

class Client: public Socket
{
    public:
        Client(std::string const& host, int const port);
        virtual bool SendMessage(std::string const& buffer) override;
        virtual bool RecvMessage(std::string & buffer) override;
};
