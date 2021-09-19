#pragma once
#include <thread>
#include "../../common/message.h"
// #include "../include/SafeQueue.hpp"
#include "locklessQ.hpp"

class MessageReader
{
private:
    std::shared_ptr<moodycamel::ReaderWriterQueue<message> > sq;
    std::unique_ptr<std::thread> reader;
    static void run_forever(std::shared_ptr<bool> running, std::shared_ptr<moodycamel::ReaderWriterQueue<message> > sq);
    std::shared_ptr<bool> running;
public:
    void run();
    bool getMessage(message &m);
    bool q_empty();
    MessageReader();
    ~MessageReader();
};
