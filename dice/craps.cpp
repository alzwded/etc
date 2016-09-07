#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

std::tuple<int, int, int> roll()
{
    static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<int> distribution(1, 6);

    auto d1 = distribution(generator);
    auto d2 = distribution(generator);
    auto sum = d1 + d2;
    return std::make_tuple(d1, d2, sum);
}

int main()
{
    enum {
        OFF,
        ON
    } state = OFF;
    int point = 0;
    int n = 0;
    while(1)
    {
        auto r = roll();
        std::cout << "[" << std::get<0>(r) << "][" << std::get<1>(r) << "] ";

        switch(state)
        {
        case ON:
            if(std::get<2>(r) == point) {
                std::cout << "Pass\n";
                state = OFF;
            } else if(std::get<2>(r) == 7) {
                std::cout << "Don't pass\n";
                state = OFF;
            } else if(++n > 8) {
                std::cout << "\n . . . ";
                n = 1;
            }
            break;
        case OFF:
            switch(std::get<2>(r))
            {
            case 7:
            case 11:
                std::cout << "Pass\n";
                break;
            case 2:
            case 3:
                std::cout << "Don't pass\n";
                break;
            case 12:
                std::cout << "Push\n";
                break;
            default:
                std::cout << "Point\n";
                state = ON;
                n = 0;
                break;
            }
            break;
        }

        std::cout << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
}
