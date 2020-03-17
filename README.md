# Client et serveur TCP avec un thread par client

## Compiler les programmes

Le serveur se compile avec la commande suivante :

`` g++ server.cpp -o server.out ``

Le client se compile avec la commande suivante :

`` g++ client.cpp -o client.out ``

## Tester le serveur TCP

La commande suivante permet de tester le serveur : 

`` curl -v telnet://127.0.0.1:3333 ``

## TODO :
- Ajouter le client
- Ajouter le multithread
- Ajouter des classes et les principes POO
- Utiliser le mécanisme d'erreur C++