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
        void addToHBTracker(int newSocket);
        void removeFromHBTracker(int sockId);
    public:
        Server(std::string const & hostname, int const port);
        void accept();
        void sendToAll(message m);
        void sendToClient(int fc, std::string msg);
        void checkStatusAndDiconnect();
        void updateHBTracker(int fd, std::chrono::steady_clock::time_point new_time);
        std::vector<Socket>& getClients();
};
