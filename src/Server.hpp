#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

class Server
{
    private :
        int serverSocketfd;
        sockaddr_in serverAddr;
        char buffer[256];

        void initSocket(int);
        void readSocket(int, char *);
        void writeSocket(int, char *);

    public : 
        Server(int);
        void run();
};

#endif