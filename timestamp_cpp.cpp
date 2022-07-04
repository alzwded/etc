#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

std::string timestamp()
{
    auto n = std::chrono::system_clock::now();
    auto t_c = std::chrono::system_clock::to_time_t(n);
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&t_c), "%x %X");
    return ss.str();
}

int main()
{
    std::cout << timestamp() << std::endl;
}
