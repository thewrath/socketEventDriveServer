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
#include <mutex>
#include <condition_variable>
#include <queue>

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
        int description;
        std::string data;
    };

    class Socket
    {
        protected: 
            int description;
            sockaddr_in address;

        public:
            Socket(unsigned int);
            static void write(Packet);

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

    typedef void (* processPacket)(Packet packet);

    class ThreadPool {
        private:
            std::vector<std::thread> threads;
            std::queue<Packet> packets;
            std::mutex queueMutex;
            std::condition_variable condition;

            bool terminate_pool = false;
            bool stopped = false;

        public:
            ThreadPool(int, processPacket);
            ~ThreadPool();
            void addPacket(Packet);
            void shutdown();

            static void threadWork(int, bool*, std::condition_variable*, std::mutex*, std::queue<Packet>*, processPacket);
    };

    class Server : public ISocketEventListener
    {
        private:
            ServerSocket masterSocket;
            ThreadPool threadPool;

        public:
            Server(unsigned int, int, processPacket);

            void onConnect(int fd) override;
            void onDisconnect(int fd) override;
            void onDataSend(int fd, const std::vector<char>& data) override;
            void onDataReceive(int fd, const std::vector<char>& data) override;
            void onSocketException(SocketException exception) override;

    };
};

#endif