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

#include <sys/epoll.h>
#include <fcntl.h>

namespace Communication 
{

    struct SocketException : public std::exception
    {
        std::string message;
        SocketException(std::string m) : message(m) {}
        ~SocketException() throw () {}
        const char* what() const throw() { return message.c_str(); }
    };

    class ISocketEventListener {

        public:
            virtual void onConnect(int fd);
            virtual void onDisconnect(int fd);
            virtual void onDataSend(int fd, const std::vector<char>& data);
            virtual void onDataReceive(int fd, const std::vector<char>& data);
            virtual void onSocketException(SocketException exception);
    };

    struct Packet
    {
        std::string data;
    };

    class Socket
    {
        protected: 
            int description;
            sockaddr_in address;

        public:
            Socket(unsigned int);
            void read(char *);
            void write(Packet);

            static void setNonBlocking(int);
            
    };

    class ServerSocket : public Socket
    {
        private:
            std::vector<ISocketEventListener *> eventListeners;
            int epollFd;
            struct epoll_event ev;
            struct epoll_event events[20];
            bool running = true;

            void handleListeningFileDescriptorActivity();
            void handleClientFileDescriptorActivity(int);

            void raiseDataSendEvent(const std::vector<char> &, int) const;
            void raiseDataReceivedEvent(const std::vector<char> &, int) const;
            void raiseClientConnectedEvent(int) const;
            void raiseClientDisconnectedEvent(int) const;
            void raiseSocketExceptionEvent(SocketException) const;

        public:
            ServerSocket(unsigned int);
            void run();
            void addEventListener(ISocketEventListener *);
            void removeEventListener(ISocketEventListener *);
    };

    class ClientSocket : public Socket
    {
        public:
            ClientSocket(unsigned int);
    };

    class Server : public ISocketEventListener
    {
        private:
            ServerSocket masterSocket;

        public:
            Server(unsigned int);
            void onConnect(int fd) override;
            void onDisconnect(int fd) override;
            void onDataSend(int fd, const std::vector<char>& data) override;
            void onDataReceive(int fd, const std::vector<char>& data) override;
            void onSocketException(SocketException exception) override;

    };
};

#endif