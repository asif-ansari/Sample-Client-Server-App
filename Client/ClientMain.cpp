#include "../Socket/include/Client.h"
#include "../Server/include/locklessQ.hpp"

#include <iostream>
#include <sstream>
#include <thread>
#include <cstring>
#include <fstream>
#include <unistd.h>


void dump_msg_thread(std::shared_ptr<moodycamel::ReaderWriterQueue<std::string> > sq, std::shared_ptr<bool> running)
{
    // Append pid if multiple clients are running in the same directory
    // NOT GUARANTEED to be unique
    pid_t pid = getpid();
    std::stringstream file_name;
    file_name<<"md_out_"<<pid<<".txt";
    
    std::string filename_txt(file_name.str());
    std::ofstream fout_txt(filename_txt, std::ios::binary);
    while(*running)
    {
        std::string tmp;
        bool success = sq->try_dequeue(tmp);
        if(success)
        {
            tmp.pop_back();
            fout_txt<<tmp<<"\n";
        }
        fout_txt.flush();
    }
    std::cout<<"dumping to file\n";
    fout_txt.close();
}

void heartbeat_handler(Client *c)
{
    while (true)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - c->getLastActive()).count();
        if(elapsed > 30000) // Haven't heard anything from server in past 30 seconds
        {
            c->SendMessage("AreYouThere?");
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: client <host> <port>\n";
        exit(1);
    }
    Client  client(argv[1], atoi(argv[2]));
    auto sq = std::make_shared<moodycamel::ReaderWriterQueue<std::string> >();
    std::shared_ptr<bool> running = std::make_shared<bool>(true);
    std::thread dumper(dump_msg_thread, sq, running);
    std::thread hbThread(heartbeat_handler, &client);

    while (true)
    {
        std::string buf;
        bool success = client.RecvMessage(buf);
        std::cout<<"New Message -> "<<buf<<'\n';
        if(buf == "Yes")
        {
            client.setLastActive(std::chrono::steady_clock::now());
            continue;
        }
        if(!success)
        {
            std::stringstream message("Failed: sendMessage()\n");
            message << strerror(errno)<<"\n";
            *running = false;
            std::cout<<message.str();
            // abort();
            continue;
        }
        bool queued = sq->enqueue(buf);
        if(!queued)
        {
            std::cout<<"Queue failed\n";
            abort();
        }
    }
    *running = false;
    std::cout<<"Shutdown "<<*running<<"\n";
    dumper.join();
    std::cout<<"Dumpper joined\n";
}