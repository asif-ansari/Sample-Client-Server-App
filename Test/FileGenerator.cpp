#include <random>
#include <fstream>
#include <sstream>
#include <iostream>

int main(int argc, char* argv[])
{
    std::string filename_txt("md_in.txt");
    int nTests = 1000;
    
    std::random_device seed;
    std::mt19937 mte(seed());  // mersenne_twister_engine
    
    std::uniform_int_distribution<> ticker_generator(1, 10);
    std::uniform_int_distribution<> price_generator(100, 200);
    std::uniform_int_distribution<> quantity_generator(10, 50);

    std::ofstream fout_txt(filename_txt, std::ios::binary);
    std::stringstream ss;

    for(int i = 0; i < nTests; ++i)
    {
        uint16_t t = ticker_generator(mte);
        uint16_t p = price_generator(mte);
        uint16_t q = quantity_generator(mte);
        fout_txt<<t<<','<<p<<','<<q<<'\n';
    }
    fout_txt.close();
}