#include "../Socket/include/Client.h"
#include "../Server/include/SafeQueue.hpp"

#include <iostream>
#include <sstream>
#include <thread>
#include <cstring>
#include <fstream>
#include <unistd.h>


void dump_msg_thread(std::shared_ptr<SafeQueue<std::string> > sq, std::shared_ptr<bool> running)
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
        if(sq->empty())
        {
            continue;
        }
        std::string tmp = sq->dequeue();
        tmp.pop_back();
        fout_txt<<tmp<<"\n";
    }
    fout_txt.close();
}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: client <host> <port>\n";
        exit(1);
    }
    std::shared_ptr<SafeQueue<std::string> > sq = std::make_shared<SafeQueue<std::string> >();
    std::shared_ptr<bool> running = std::make_shared<bool>(true);
    std::thread dumper(dump_msg_thread, sq, running);

    try
    {
        Client  client(argv[1], atoi(argv[2]));

        while (true)
        {
            std::string buf;
            bool success = client.RecvMessage(buf);
            std::cout<<"New Message -> "<<buf<<'\n';
            sq->enqueue(buf);
            if(!success)
            {
                std::stringstream message("Failed: sendMessage()\n");
                message << strerror(errno);
                *running = false;
                throw std::runtime_error(message.str());
            }
        }
        std::cout<<"Exiting\n";
    }
    catch(std::exception const& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    *running = false;
    std::cout<<"Shutdown "<<*running<<"\n";
    dumper.join();
    std::cout<<"Dumpper joined\n";
}