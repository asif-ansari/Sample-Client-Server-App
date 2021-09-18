#pragma once
#include <thread>
#include "../../common/message.h"
#include "../include/SafeQueue.hpp"

class MessageReader
{
private:
    std::shared_ptr<SafeQueue<message> > sq;
    std::unique_ptr<std::thread> reader;
    static void run_forever(bool running, std::shared_ptr<SafeQueue<message> > sq);
    bool running;
public:
    void run();
    message getMessage();
    MessageReader();
    ~MessageReader();
};
