#include "Communication.hpp" 

int main()
{
    Communication::Server server(3333, std::thread::hardware_concurrency());
    return 0;
}