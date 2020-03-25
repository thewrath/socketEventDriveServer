#include "Serveur.hpp"

const int BUFSIZE = 4096;

namespace Communication
{

    Prise::Prise(int port)
    {
        // creation d'un socket de type TCP
        this->description = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        
        if (this->description < 0) {
            throw "Erreur initialisation socket.";
        }

        // Structure contenant les informations "reseaux" de la socket
        this->adresse.sin_addr.s_addr = INADDR_ANY; // on accepte n'importe qu'elle source entrante
        this->adresse.sin_port = htons(port); // on precise le port d'ecoute (htons permet de convertir le int du c++ en int reseaux)
        this->adresse.sin_family = AF_INET; // on precise qu'il s'agit d'un socket reseau et non inter-processus (AF_UNIX)
    }

    Prise::Prise(int description, sockaddr_in adresse)
    {
        // creation d'un socket de type TCP
        this->description = description;
        this->adresse = adresse;
    }

    void Prise::lier()
    {
        if (bind(this->description, (struct sockaddr*) &this->adresse, sizeof(this->adresse))) {
            throw "Impossible d'ouvrir le socket.";
        }
    }

    void Prise::ecouter()
    {
        listen(this->description, 5);
    }

    void Prise::lire(char *buffer)
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

    void Prise::ecrire(Paquet paquet)
    {
        std::cout << "Send response to client :" << paquet.donnee << std::endl;
        int n = send(this->description, paquet.donnee.c_str(), paquet.donnee.length(), 0);
        if (n < 0) {
            throw "ERROR writing to socket";
        }
    }

    Prise Prise::accepter()
    {
        sockaddr_in adresse;
        socklen_t clilen = sizeof(adresse);
        int description = accept(this->description, (struct sockaddr *) &adresse, &clilen);
    
        return Prise(description, adresse); 
    }

    Prise::~Prise()
    {
        close(this->description);
    }

    Serveur::Serveur(int port) : prisePrincipale(port)
    {
        this->prisePrincipale.lier();
    }

    void Serveur::lancer()
    {
        this->prisePrincipale.ecouter();
        // on prepare un emplacement pour stocker les informations du client
        while(true) {
            Prise priseClient = this->prisePrincipale.accepter(); 
        
            std::thread client(Serveur::gererConnexion, priseClient);
            this->clients.push_back(std::move(client));
        }

        for(std::thread & client : this->clients) {
            if(client.joinable()) {
                client.join();
            }
        }
    }

    void Serveur::gererConnexion(Prise priseClient)
    {
        char buffer[256];
        std::cout << "New client connected with socket : " << priseClient.description << std::endl;

        while(priseClient.description >= 0)
        {
            priseClient.lire(buffer);
            priseClient.ecrire(Paquet{"Hello from server"});
        }
    }

}