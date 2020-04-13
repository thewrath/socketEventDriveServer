#include "Communication.hpp"

const int BUFSIZE = 4096;

namespace Communication
{
    void ISocketEventListener::onConnect(int fd) {}
    void ISocketEventListener::onDisconnect(int fd) {}
    void ISocketEventListener::onDataSend(int fd, const std::vector<char>& data) {}
    void ISocketEventListener::onDataReceive(int fd, const std::vector<char>& data) {}
    void ISocketEventListener::onSocketException(SocketException exception) {}

    Socket::Socket(unsigned int port)
    {
        this->description = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (this->description < 0) {
            throw "Erreur initialisation socket.";
        }

        // Structure contenant les informations "reseaux" de la socket
        this->address.sin_addr.s_addr = INADDR_ANY; // on accepte n'importe qu'elle source entrante
        this->address.sin_port = htons(port); // on precise le port d'ecoute (htons permet de convertir le int du c++ en int reseaux)
        this->address.sin_family = AF_INET; // on precise qu'il s'agit d'un socket reseau et non inter-processus (AF_UNIX)

    }

    void Socket::read(char *buffer)
    {
        ssize_t bytes_read = 0;

        //testing stuff, BUFSIZE = 4096
        printf("Received Socket # %d\nBuffer Size = %d\n", this->description, BUFSIZE);

        buffer[bytes_read] = 0; // Null-terminate the buffer
        bytes_read = recv(this->description, buffer, BUFSIZE - 1, 0);
        printf("Buffer content: %s\n", buffer);

        std::cout << "Fin de reception" << std::endl;
        if (bytes_read == -1) {
            throw "Socket recv failed";
        }
    }

    void Socket::write(Packet packet)
    {
        std::cout << "Send response to client :" << packet.data << std::endl;
        int n = send(this->description, packet.data.c_str(), packet.data.length(), 0);
        if (n < 0) {
            throw "ERROR writing to socket";
        }
    }

    ServerSocket::ServerSocket(unsigned int port) : Socket(port)
    {
        if ((this->epollFd = epoll_create1(0)) < 0) {
            throw "Failed to create epoll instance";
        }

        if (bind(this->description, (struct sockaddr*) &this->address, sizeof(this->address))) {
            throw "Impossible d'ouvrir le socket.";
        }

    }

    void ServerSocket::run()
    {
        if (listen(this->description, 10) < 0) {
            throw "Failed to start listening.";
        }

        this->ev.data.fd = this->description;
        this->ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP | EPOLLET;
        if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this->description, &this->ev) < 0) {
            throw "Failed to configure epoll file descriptor";
        }

        while (this->running) {

            // Get number of file descriptors that are ready for I/O
            int nfds = epoll_wait(this->epollFd, this->events, 20, -1);

            if (nfds == -1) {
                close(nfds);
                close(this->description);
                throw "epoll_wait() failed to wait for events.";
            }

            // Iterate all events
            for (int i = 0; i < nfds; ++i) {
                try {
                    if (events[i].data.fd == this->description){
                        handleListeningFileDescriptorActivity();
                    } else{
                        handleClientFileDescriptorActivity(i);
                    }
                } catch (SocketException& e) {
                    this->raiseSocketExceptionEvent(e);
                }
            }
        }
    }

    void ServerSocket::handleListeningFileDescriptorActivity() {

        // accept client connection
        socklen_t addressLen = sizeof(this->address);
        int fd = accept(this->description, (struct sockaddr *) &this->address, &addressLen);
        if (fd < 0) {
            throw SocketException("Invalid file descriptor from accepted connection ");
        }

        // Set fd to be non blocking
        // setNonBlocking(fd);

        // Monitor read operations, set edge-triggered
        this->ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
        this->ev.data.fd = fd;

        // Add fd to epoll data structure
        if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, fd, &this->ev) < 0) {
            throw SocketException("Failed to add connected socket fd to epoll");
        }

        // Notify that a new client connected
        this->raiseClientConnectedEvent(fd);
    }

    void ServerSocket::handleClientFileDescriptorActivity(int index) {
        if (this->events[index].events & EPOLLIN) {

            // Invalid filedescriptor
            if (this->events[index].data.fd < 0) {
                throw SocketException("Not a valid filedescriptor.");
            }

            // Data available for reading
            char buffer[512];
            ssize_t result = recv(this->events[index].data.fd, buffer, sizeof(buffer), 0);

            if (result < 0) {
                if (errno == ECONNRESET) {
                    close(events[index].data.fd);
                    throw SocketException("Failed to receive data");
                }
            } else if (result == 0) {
                close(events[index].data.fd);
                this->raiseClientDisconnectedEvent(events[index].data.fd);

            } else {
                ev.events = EPOLLOUT | EPOLLIN | EPOLLET;
                std::vector<char> data(buffer, buffer + result);
                this->raiseDataReceivedEvent(data , events[index].data.fd);
            }
        }
        if (this->events[index].events & EPOLLOUT) {
            // TODO Implement
            std::cout << "EPOLLOUT" << std::endl;
        }
        if (this->events[index].events & EPOLLRDHUP) {
            // TODO Implement
            std::cout << "EPOLLRDHUP" << std::endl;
        }
        if (this->events[index].events & EPOLLHUP) {
            // TODO Implement
            std::cout << "EPOLLHUP" << std::endl;
        }
        if (this->events[index].events & EPOLLERR) {
            close(this->events[index].data.fd);
            throw SocketException("Unexpected exception occured EPOLLERR");
        }
    }

    void ServerSocket::removeEventListener(ISocketEventListener *listener) {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            if (eventListeners[i] == listener) {
                eventListeners.erase(eventListeners.begin() + i);
                break;
            }
        }
    }

    void ServerSocket::addEventListener(ISocketEventListener *listener) {
        eventListeners.push_back(listener);
    }

    // Socket Events
    void ServerSocket::raiseClientDisconnectedEvent(int socket) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onDisconnect(socket);
        }
    }

    void ServerSocket::raiseClientConnectedEvent(int socket) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onConnect(socket);
        }
    }

    void ServerSocket::raiseDataSendEvent(const std::vector<char> &data, int fd) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onDataSend(fd, data);
        }
    }

    void ServerSocket::raiseDataReceivedEvent(const std::vector<char> &data, int fd) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onDataReceive(fd, data);
        }
    }

    void ServerSocket::raiseSocketExceptionEvent(SocketException exception) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onSocketException(exception);
        }
    }

    ClientSocket::ClientSocket(unsigned int port) : Socket(port)
    {

    }

    Server::Server(unsigned int port) : masterSocket(port)
    {
        this->masterSocket.addEventListener(this);
        this->masterSocket.run();
    }

    void Server::onConnect(int fd)
    {
        std::cout << "New client connected" << std::endl;
    }

    void Server::onDisconnect(int fd)
    {
        std::cout << "Client disconnected" << std::endl;
    }

    void Server::onDataSend(int fd, const std::vector<char>& data)
    {
        std::cout << "Client sending data." << std::endl;
    }

    void Server::onDataReceive(int fd, const std::vector<char>& data)
    {
        std::cout << "Received from fd " << fd << ": ";
        for (std::vector<char>::const_iterator i = data.begin(); i != data.end(); i++) {
            std::cout << *i;
        }

        std::cout << std::endl;
    }

    void Server::onSocketException(SocketException exception)
    {
        std::cout << exception.message << std::endl;
    }
}
