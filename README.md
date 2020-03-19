# Client et serveur TCP avec un thread par client

## Compiler les programmes

Le serveur se compile avec la commande suivante :

`` make server ``

Le client se compile avec la commande suivante :

`` make client ``

## Tester le serveur TCP

La commande suivante permet de tester le serveur : 

``telnet -r localhost 3333 ``

## TODO :
- Ajouter le client
- Ajouter le multithread
- Utiliser le mécanisme d'erreur C++
- Implémenter une architecture orientée évenements
- Ajouter le vérification de connexion et les reconnexions en cas de perte