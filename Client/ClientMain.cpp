#include "../Socket/include/Client.h"
#include "lrb.hpp"

#include <iostream>
#include <sstream>
#include <cstdint>
#include <chrono>
#include <thread>
#include <cstring>


jnk0le::Ringbuffer<std::string, 256> message;

void dump_msg_thread()
{
    while (true)
    {
        std::string *tmp = nullptr;
		while(!message.remove(tmp));
        {
		    printf("%s fired\n", tmp);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout<<"REASDD "<<*tmp<<"\n";
    }
    
    // MessageReader m;
    // m.run();
    // while (true)
    // {
    //     message msg = m.getMessage();
    //     server->sendToAll(msg);
    // }
    // std::cout<<"messge sender shutdown\n";
}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: client <host> <port>\n";
        exit(1);
    }
    std::thread dumper(dump_msg_thread);

    try
    {
        Client  client(argv[1], atoi(argv[2]));
        // int i = 0;
        // while(true)
        // {
        //     // bool success = client.SendMessage(std::to_string(i));
        //     if(!success)
        //     {
        //         std::stringstream message("Failed: sendMessage()\n");
        //         message << strerror(errno);
        //         throw std::runtime_error(message.str());
        //     }
        //     std::cout<<"Sleeping "<<i<<"\n";
        //     std::this_thread::sleep_for(std::chrono::seconds(3));
        //     i++;
        // }

        while (true)
        {
            std::string buf;
            // std::cout<<"before Recv client "<<buf<<'\n';
            bool success = client.RecvMessage(buf);
            std::cout<<"New Message -> "<<buf<<'\n';
            message.insert(buf);
            if(!success)
            {
                std::stringstream message("Failed: sendMessage()\n");
                message << strerror(errno);
                throw std::runtime_error(message.str());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            // std::cout<<buf<<'\n';
        }
        
        std::cout<<"Exiting\n";
    }
    catch(std::exception const& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    std::cout<<"Shutdown\n";
    dumper.join();
}