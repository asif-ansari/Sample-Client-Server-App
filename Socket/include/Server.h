#include "Socket.h"
#include "../../common/message.h"
#include <vector>
#include <map>
#include <chrono>

class Server: public Socket
{
    private:
        const int max_queued_requests = 5;
        std::vector<Socket> clients;
        std::map<int, std::chrono::steady_clock::time_point> heartbeat_tracker;
    public:
        Server(std::string const & hostname, int const port);
        void accept();
        void sendToAll(message m);
        void checkStatusAndDiconnect();
        std::vector<Socket>& getClients();
};
