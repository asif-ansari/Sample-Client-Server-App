#include <iostream>

int main()
{
    std::string str;
    while(std::getline(std::cin, str))
        std::cout<<str;
    return 0;
}