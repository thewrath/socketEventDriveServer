#include "Server.hpp"

const int BUFSIZE = 4096;

namespace TcpServer
{
    Server::Server(int port)
    {
        this->initSocket(port);
    }

    Server::~Server()
    {
        close(this->serverSocketfd);
    }

    void Server::initSocket(int port)
    {
        // creation d'un socket de type TCP
        this->serverSocketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        
        if (this->serverSocketfd < 0) {
            throw "Erreur initialisation socket";
        }
        // Structure contenant les informations "reseaux" de la socket

        this->serverAddr.sin_addr.s_addr = INADDR_ANY; // on accepte n'importe qu'elle source entrante
        this->serverAddr.sin_port = htons(port); // on precise le port d'ecoute (htons permet de convertir le int du c++ en int reseaux)
        this->serverAddr.sin_family = AF_INET; // on precise qu'il s'agit d'un socket reseau et non inter-processus (AF_UNIX)
        
        int res = bind(this->serverSocketfd, (struct sockaddr*) &this->serverAddr, sizeof(this->serverAddr)); // on applique la configuration au socket server
        if (res < 0) {
            throw "Impossible d'ouvrir le socket serveur.";
        }
    }

    void Server::run()
    {
        
        listen(this->serverSocketfd, 5);
        // on prepare un emplacement pour stocker les informations du client
        while(true)
        {
            sockaddr_in cliAddr; 
            socklen_t clilen = sizeof(cliAddr);
            int clientSocketfd = accept(this->serverSocketfd, (struct sockaddr *) &cliAddr, &clilen);

            std::thread clientThread(Server::handleConnection, clientSocketfd);
            this->clientThreads.push_back(std::move(clientThread));
        }

        for(std::thread & clientThread : this->clientThreads) {
            if(clientThread.joinable()) {
                clientThread.join();
            }
        }
    }

    static void Server::handleConnection(int clientSocketfd)
    {
        char buffer[256];
        std::cout << "New client connected : " << std::endl;

        while(clientSocketfd >= 0)
        {
            readSocket(clientSocketfd, buffer);
            writeSocket(clientSocketfd, buffer);
        }
    }

    void static Server::readSocket(int socket, char *buffer)
    {
        ssize_t bytes_read = 0;
        
        //testing stuff, BUFSIZE = 4096
        printf("Received Socket # %d\nBuffer Size = %d\n", socket, BUFSIZE);
        
        bytes_read = recv(socket, buffer, BUFSIZE - 1, 0);
        while (bytes_read > 0) {
            buffer[bytes_read] = 0; // Null-terminate the buffer
            printf("Buffer: %s\n", buffer);	
            bytes_read = recv(socket, buffer, BUFSIZE - 1, 0);

            std::cout << bytes_read << std::endl;
        }

        std::cout << "Fin de reception" << std::endl;
        if (bytes_read == -1) {
            throw "Socket recv failed";
        }
    }

    void static Server::writeSocket(int socket, char *buffer)
    {
        std::cout << "Send response" << std::endl;
        
        char *hello = "Hello from server"; 
        int n = send(socket, hello, strlen(hello), 0);
        if (n < 0) {
            throw "ERROR writing to socket";
        }
    }
}