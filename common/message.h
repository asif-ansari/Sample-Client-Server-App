#pragma once
#include <sstream>

struct message
{
    int ticker;
    int price;
    int quantity;

    message(int t, int p, int q): ticker(t), price(p), quantity(q)
    {
        
    }
    std::string to_string()
    {
        std::stringstream ss;
        ss<<ticker<<','<<price<<','<<quantity;
        return ss.str();
    }

};
