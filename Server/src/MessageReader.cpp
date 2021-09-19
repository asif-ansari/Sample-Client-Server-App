#include "../include/MessageReader.h"
#include <iostream>

message str_to_message(const std::string &s)
{
    char delim = ',';
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    int i = 0;
    while (std::getline(ss, item, delim))
        elems.push_back(std::move(item));
    message m(atoi(elems[0].c_str()), atoi(elems[1].c_str()), atoi(elems[2].c_str()));
    return m;
}

void MessageReader::run_forever(std::shared_ptr<bool> running, std::shared_ptr<SafeQueue<message> > sq)
{
    // Wait 10 seconds before publishing anything
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    while (*running)
    {
        std::string str;
        while(std::getline(std::cin, str))
        {
            std::cout<<"New Event on exchange -> "<<str<<'\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            message m = str_to_message(str);
            sq->enqueue(m);
        }
        message m(0,0,0,true); //special EOD message
        sq->enqueue(m);
    }
    std::cout<<"reader shut down\n";
}

void MessageReader::run()
{
    *running = true;
    reader = std::make_unique<std::thread>(&(this->run_forever), running, sq);
}

message MessageReader::getMessage()
{
    return sq->dequeue();
}

bool MessageReader::q_empty()
{
    return sq->empty();
}

MessageReader::MessageReader()
{
    running = std::make_shared<bool>(false);
    sq = std::make_shared<SafeQueue<message> >();
}

MessageReader::~MessageReader()
{
    std::cout<<"Message reader destroyed\n";
    *running = false;
    reader->join();
}
