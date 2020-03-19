#include "Server.hpp"

int main()
{
    TcpServer::Server server(3333);
    server.run();
    return 0;
}