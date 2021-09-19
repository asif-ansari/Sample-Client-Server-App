#include "Socket.h"
#include <chrono>

class Client: public Socket
{
    private:
        std::chrono::steady_clock::time_point last_active;
    public:
        Client(std::string const& host, int const port);
        std::chrono::steady_clock::time_point& getLastActive();
        void setLastActive(std::chrono::steady_clock::time_point new_time);
        virtual bool SendMessage(std::string const& buffer) override;
        virtual bool RecvMessage(std::string & buffer) override;
};
