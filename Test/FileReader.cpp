#include <iostream>
#include <cstring>
#include <array>
#include <vector>

// const std::size_t INIT_BUFFER_SIZE = 1024;

// struct message
// {
//     message(uint16_t t, uint16_t p, uint16_t q): ticker(t), price(p), quantity(q) {};
//     uint16_t ticker;
//     uint16_t price;
//     uint16_t quantity;
// };

int main()
{
    // std::freopen(nullptr, "rb", stdin);
    // if(std::ferror(stdin))
    //     throw std::runtime_error(std::strerror(errno));

    // std::size_t len;
    // std::array<char, INIT_BUFFER_SIZE> buf;

    // std::vector<char> input;

    // // use std::fread and remember to only use as many bytes as are returned
    // // according to len
    // while((len = std::fread(buf.data(), sizeof(buf[0]), buf.size(), stdin)) > 0)
    // {
    //     if(std::ferror(stdin) && !std::feof(stdin))
    //         throw std::runtime_error(std::strerror(errno));

    //     // use {buf.data(), buf.data() + len} here
    //     input.insert(input.end(), buf.data(), buf.data() + len); // append to vector
    // }
    std::string str;
    while(std::getline(std::cin, str))
        std::cout<<str;
    return 0;
}