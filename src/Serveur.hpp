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

namespace Communication 
{
    struct Paquet
    {
        std::string donnee;
    };

    class Prise
    {
        public:
            int description;
            sockaddr_in adresse;

            Prise(int);
            Prise(int, sockaddr_in);
            ~Prise();

            Prise accepter();
            
            void lier();
            void ecouter();

            void lire(char *);
            void ecrire(Paquet);

    };

    class Serveur
    {
        private :
            Prise prisePrincipale;
            std::vector<std::thread> clients;
            
            static void gererConnexion(Prise);

        public : 
            Serveur(int);
            void lancer();
    };
};

#endif