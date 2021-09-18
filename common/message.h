#pragma once
#include <sstream>

struct message
{
    int ticker;
    int price;
    int quantity;
    bool eod;

    message(int t, int p, int q): ticker(t), price(p), quantity(q), eod(false) { }
    message(int t, int p, int q, bool e): ticker(t), price(p), quantity(q), eod(e) { }
    
    std::string to_string()
    {
        std::stringstream ss;
        ss<<ticker<<','<<price<<','<<quantity;
        return ss.str();
    }

};
