/**
 * \file Communication.hpp
 * \brief Package de communication
 * \author thewrath
 * \version 1.0
 * \date 20/04/2020
 *
 * Package pour la communication en TCP avec plusieurs clients.
 *
 */

#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>


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
            Socket(unsigned int, std::string);
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
            ClientSocket(unsigned int, std::string);
    };

    /**
     * @brief Function definition, need to be implements by the businness part of the server (it receive a packet to process)
     * 
     */
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

            void log(std::string);

        public:
            Server(unsigned int, unsigned int, int, processPacket);

            void onConnect(int fd) override;
            void onDisconnect(int fd) override;
            void onDataSend(int fd, const std::vector<char>& data) override;
            void onDataReceive(int fd, const std::vector<char>& data) override;
            void onSocketException(SocketException exception) override;

    };
};

#endif