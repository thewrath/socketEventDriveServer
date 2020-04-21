#include "Communication.hpp"

/**
 * @brief Contains all stuff for TCP communication with multi-clients
 * 
 */
namespace Communication
{
    
    /**
     * @brief Call on every client connected event 
     * 
     * @param fd socket file description
     */
    void ISocketEventListener::onConnect(int fd) {}

    /**
     * @brief Call on every client disconnected event
     * 
     * @param fd socket file description
     */
    void ISocketEventListener::onDisconnect(int fd) {}

    /**
     * @brief Call on every data sent event
     * 
     * @param fd socket file description
     * @param data data attach to the send event
     */
    void ISocketEventListener::onDataSend(int fd, const std::vector<char>& data) {}

    /**
     * @brief Call on every data received event
     * 
     * @param fd Socket file description
     * @param data data attach to the receive event
     */
    void ISocketEventListener::onDataReceive(int fd, const std::vector<char>& data) {}

    /**
     * @brief Call on every exception event
     * 
     * @param exception exception to manage
     */
    void ISocketEventListener::onSocketException(SocketException exception) {}

    /**
     * @brief Construct a new Socket:: Socket object
     * 
     * @param port
     */
    Socket::Socket(unsigned int port)
    {
        this->description = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (this->description < 0) {
            throw SocketException("Erreur initialisation socket.");
        }

        this->address.sin_addr.s_addr = INADDR_ANY;
        this->address.sin_port = htons(port);
        this->address.sin_family = AF_INET;

    }

    /**
     * @brief Procedure to write on socket designed by description attribut in packet
     * 
     * @param packet the packet to write on the TCP connection
     */
    void Socket::write(Packet packet)
    {
        int n = send(packet.description, packet.data.c_str(), strlen(packet.data.c_str()) + 1, 0);
        if (n < 0) {
            throw SocketException("ERROR writing to socket");
        }
    }

    /**
     * @brief Procedure to set socket designed by fg argument in non-blocking mode
     * 
     * @param fd socket file description
     */
    void Socket::setNonBlocking(int fd) {
        int options;
        if ((options = fcntl(fd, F_GETFL)) < 0) {
            throw SocketException("F_GETFL failed.");
        }
        options = options | O_NONBLOCK;
        if (fcntl(fd, F_SETFL, options) < 0) {
            throw SocketException("F_SETFL failed.");
        }
    }

    /**
     * @brief Construct a new Server Socket:: Server Socket object
     * 
     * @param port the port to listen on
     */
    ServerSocket::ServerSocket(unsigned int port) : Socket(port)
    {

        Socket::setNonBlocking(this->description);

        int optval = 1;
        int optlen = sizeof(optval);
        if(setsockopt(this->description, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
            throw SocketException("Cannot set option on socket");
        }

        if ((this->epollFd = epoll_create1(0)) < 0) {
            throw SocketException("Failed to create epoll instance");
        }

        if (bind(this->description, (struct sockaddr*) &this->address, sizeof(this->address))) {
            throw SocketException("Impossible d'ouvrir le socket.");
        }

    }

    /**
     * @brief configure EPOLL file descriptors and listen for EPOLL events and log messages   
     * 
     */
    void ServerSocket::run()
    {
        if (listen(this->description, 10) < 0) {
            throw SocketException("Failed to start listening.");
        }

        this->ev.data.fd = this->description;
        this->ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP | EPOLLET;
        if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this->description, &this->ev) < 0) {
            throw SocketException("Failed to configure epoll file descriptor");
        }

        while (this->running) {

            // Get number of file descriptors that are ready for I/O
            int nfds = epoll_wait(this->epollFd, this->events, 20, -1);

            if (nfds == -1) {
                close(nfds);
                close(this->description);
                throw SocketException("epoll_wait() failed to wait for events.");
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

    /**
     * @brief handler to process EPOLL main socket file descriptor activities 
     * 
     */
    void ServerSocket::handleListeningFileDescriptorActivity() {

        // Accept client connection
        socklen_t addressLen = sizeof(this->address);
        int fd = accept(this->description, (struct sockaddr *) &this->address, &addressLen);
        if (fd < 0) {
            throw SocketException("Invalid file descriptor from accepted connection ");
        }

        // Set fd to be non blocking, this function raised error if failed
        Socket::setNonBlocking(fd);

        // Monitor read and write operations, set edge-triggered
        this->ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
        this->ev.data.fd = fd;

        // Add fd to epoll data structure
        if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, fd, &this->ev) < 0) {
            throw SocketException("Failed to add connected socket fd to epoll");
        }

        // Notify that a new client connected
        this->raiseClientConnectedEvent(fd);
    }

    /**
     * @brief Handler to process EPOLL client socket file descriptor activities 
     * 
     * @param index event index in the events array
     */
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

    /**
     * @brief Remove listener from EPOLL events listeners
     * 
     * @param listener the listener to remove 
     */
    void ServerSocket::removeEventListener(ISocketEventListener *listener) {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            if (eventListeners[i] == listener) {
                eventListeners.erase(eventListeners.begin() + i);
                break;
            }
        }
    }

    /**
     * @brief Add listener to EPOLL events listeners
     * 
     * @param listener the listener to add 
     */
    void ServerSocket::addEventListener(ISocketEventListener *listener) {
        eventListeners.push_back(listener);
    }

    /**
     * @brief Dispatch client disconnected event 
     * 
     * @param fd socket file description 
     */
    void ServerSocket::raiseClientDisconnectedEvent(int fd) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onDisconnect(fd);
        }
    }

    /**
     * @brief Dispatch client connected event
     * 
     * @param fd socket file description
     */
    void ServerSocket::raiseClientConnectedEvent(int fd) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onConnect(fd);
        }
    }

    /**
     * @brief Dispatch data send event
     * 
     * @param data data attach to the event 
     * @param fd socket file description
     */
    void ServerSocket::raiseDataSendEvent(const std::vector<char> &data, int fd) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onDataSend(fd, data);
        }
    }

    /**
     * @brief Dispatch data received event
     * 
     * @param data data attach to the event
     * @param fd socket file description
     */
    void ServerSocket::raiseDataReceivedEvent(const std::vector<char> &data, int fd) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onDataReceive(fd, data);
        }
    }

    /**
     * @brief Dispatch EPOLL exception event
     * 
     * @param exception exception to raised
     */
    void ServerSocket::raiseSocketExceptionEvent(SocketException exception) const {
        for (unsigned int i = 0; i < eventListeners.size(); i++) {
            eventListeners[i]->onSocketException(exception);
        }
    }

    /**
     * @brief Construct a new Client Socket:: Client Socket object
     * 
     * @param port client selected port
     */
    ClientSocket::ClientSocket(unsigned int port) : Socket(port)
    {

    }

    /**
     * @brief Construct a new Thread Pool:: Thread Pool object
     * 
     * @param number_of_thread number of thread to be used by the pool
     * @param process how a packet is process
     */
    ThreadPool::ThreadPool(int number_of_thread, processPacket process)
    {
        for(int i = 0; i < number_of_thread; i++)
        {  
            this->threads.push_back(std::thread(ThreadPool::threadWork, i, &this->terminate_pool, &this->condition, &this->queueMutex, &this->packets, process));
        }
    }

    /**
     * @brief Destroy the Thread Pool:: Thread Pool object
     * 
     */
    ThreadPool::~ThreadPool()
    {
        if(this->stopped == false){
            this->shutdown();
        }    
    }
    
    /**
     * @brief Add packet to be process by the thread poll 
     * 
     * @param packet 
     */
    void ThreadPool::addPacket(Packet packet)
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            this->packets.push(packet);
        }
        this->condition.notify_one();
    }

    /**
     * @brief Shutdown thread poll (join all thread)
     * 
     */
    void ThreadPool::shutdown()
    {
        {
            std::unique_lock<std::mutex> lock(this->queueMutex);
            this->terminate_pool = true;
        }

        this->condition.notify_all();

        for(std::thread &every_thread : this->threads)
        {   
            every_thread.join();
        }

        this->threads.clear();  
        this->stopped = true; 
    }

    /**
     * @brief Thread job (take packet in the packets queue and process it) 
     * 
     * @param threadID thread id in the pool
     * @param terminate_pool flag, if the pool want to shutdown
     * @param condition for concurrent access on the packets std::queue
     * @param queueMutex for concurrent access on the packets std::queue
     * @param packets std::queue for packets internal transmission
     * @param process how to process a packet
     */
    void ThreadPool::threadWork(int threadID, bool* terminate_pool, std::condition_variable* condition, std::mutex* queueMutex, std::queue<Packet>* packets, processPacket process)
    {
        while(true){
            {
                std::unique_lock<std::mutex> lock(*queueMutex);
                condition->wait(lock, [packets, terminate_pool]{return !packets->empty() || *terminate_pool; });
                Packet packet = packets->front();
                packets->pop();

                process(packet);
            }
        }
    }

    /**
     * @brief Construct a new Server:: Server object
     * 
     * @param port the to listen on
     * @param number_of_thread max number of thread for the pool
     * @param process how to process a packet 
     */
    Server::Server(unsigned int port, int number_of_thread, processPacket process) : masterSocket(port), threadPool(number_of_thread, process)
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
        this->threadPool.addPacket(Packet{fd, std::string(data.begin(), data.end())});
    }

    void Server::onSocketException(SocketException exception)
    {
        std::cout << exception.message << std::endl;
    }
}
