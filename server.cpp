#include <iostream>
#include <string>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int PORT = 3333;

int main()
{

    // creation d'un socket de type TCP
    int server_socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socketfd < 0) {
		std::cout << "Erreur initialisation socket";
		return 1;
	}
    // Structure contenant les informations "reseaux" de la socket
    sockaddr_in server_addr;
    // bzero((char *) &addr, sizeof(addr)); // On applique bzero sur les champs de la structure addr pour être sur qu'elle ne contient rien
    server_addr.sin_addr.s_addr = INADDR_ANY; // on accepte n'importe qu'elle source entrante
    server_addr.sin_port = htons(PORT); // on precise le port d'ecoute (htons permet de convertir le int du c++ en int reseaux)
    server_addr.sin_family = AF_INET; // on precise qu'il s'agit d'un socket reseau et non inter-processus (AF_UNIX)
    int res = bind(server_socketfd, (struct sockaddr*) &server_addr, sizeof(server_addr)); // on applique la configuration au socket server
    if (res < 0) {
        std::cout << "Impossible d'ouvrir le socket serveur.";
        return 1;
    }

    // On boucle pour eviter que le serveur ne se ferme
    while(true)
    {
        listen(server_socketfd, 5);
        // on prepare un emplacement pour stocker les informations du client
        sockaddr_in cli_addr; 
        socklen_t clilen = sizeof(cli_addr);
        int client_socketfd = accept(server_socketfd, (struct sockaddr *) &cli_addr, &clilen);
        if (client_socketfd < 0) {
            perror("ERROR on accept");
        } else {
            std::cout << "New client connected" << std::endl;    
        }
    }

    return 0;
}
