#include "Serveur.hpp"

int main()
{
    Communication::Serveur serveur(3333);
    serveur.lancer();
    return 0;
}