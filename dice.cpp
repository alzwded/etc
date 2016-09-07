#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <string>

constexpr int maxdice = 100;

void roll(int cnt, int size)
{
    static int pcnt = 2, psize = 6;
    if(cnt > 0) pcnt = cnt;
    if(size > 0) psize = (size <= 100) ? size : 100;

    static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<int> distribution(1, maxdice);

#if 0
    std::cerr << "[" << pcnt << "d" << psize << "]\n";
#endif

    int sum = 0;
    for(auto i = 0; i < pcnt; ++i)
    {
        auto num = ((distribution(generator) % psize) + 1);
        sum += num;
        std::cout << num;
        if(i < pcnt - 1) std::cout << " + ";
        else std::cout << " = " << sum;
    }
    std::cout << std::endl << std::flush;

}

#define reroll() roll(0, 0)

int main()
{
    while(std::cin.good())
    {
        std::string s;
        std::getline(std::cin, s);
        if(!std::cin.good() || std::cin.eof()) break;

        if(s.empty())
        {
            reroll();
            continue;
        }

        std::regex r("(\\d*)[d](\\d+)");
        std::smatch m;
        if(std::regex_match(s, m, r))
        {
            auto mcnt = m[1];
            auto msize = m[2];
            int cnt = 1, size = 1;
            if(mcnt.length())
            {
                std::stringstream ss;
                ss << mcnt.str();
                ss >> cnt;
            }
            if(msize.length())
            {
                std::stringstream ss;
                ss << msize.str();
                ss >> size;
            }
            roll(cnt, size);
        } else {
            std::cout << "invalid dice" << std::endl << std::flush;
        }
    }
}
