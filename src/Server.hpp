#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string.h>
#include <thread>
#include <vector>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

namespace TcpServer 
{
    class Server
    {
        private :
            int serverSocketfd;
            sockaddr_in serverAddr;
            char buffer[256];
            std::vector<std::thread> clientThreads;

            void initSocket(int);
            
            static void handleConnection(int);
            static void readSocket(int, char *);
            static void writeSocket(int, char *);


        public : 
            Server(int);
            ~Server();
            void run();
    };
};

#endif