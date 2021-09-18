#pragma once
#include <thread>
#include "../../common/message.h"
#include "../include/SafeQueue.hpp"

class MessageReader
{
private:
    std::shared_ptr<SafeQueue<message> > sq;
    std::unique_ptr<std::thread> reader;
    static void run_forever(std::shared_ptr<bool> running, std::shared_ptr<SafeQueue<message> > sq);
    std::shared_ptr<bool> running;
public:
    void run();
    message getMessage();
    bool q_empty();
    MessageReader();
    ~MessageReader();
};
