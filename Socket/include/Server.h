#include "Socket.h"
#include "../../common/message.h"
#include <vector>

class Server: public Socket
{
    private:
        const int max_queued_requests = 5;
        std::vector<Socket> clients;
    public:
        Server(std::string const & hostname, int const port);
        void accept();
        void sendToAll(message m);
};
