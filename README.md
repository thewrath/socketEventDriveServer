# Client et serveur TCP avec un thread par client

## Compiler les programmes

Créez le dossier bin

`` mkdir bin ``

Le serveur se compile avec la commande suivante :

`` make server ``

Le client se compile avec la commande suivante :

`` make client ``

## Tester le serveur TCP

La commande suivante permet de tester le serveur :

``telnet -r localhost 3333 ``

## Utilisation de EPOLL
La librairie système EPOLL (sur Linux) est utilisée pour offrir une gestion asynchrone des sockets. L'application demande à EPOLL de la notifiée des différents événents liés à certains descripteurs de fichiers (les sockets nottament). L'intêret dans notre application est d'avoir le thread principal qui est responsable de la gestion de l'ensemble des E/S (Connexion entrante, communication vers n clients, socket sortant pour les logs et l'interface graphique...). Lorsque que le thread principal à un travail lourd à éffectuer (autre que la gestion d'une E/S) il le délègue un thread provenant d'un pool de threads (avec une taille limité). L'idée, ici, n'est pas de viser les performances, mais de mettre en place un architecture autre que le simple un thread/client. L'architecture présentée ici a l'avantage de rassembler l'ensemble de la gestion des E/S à un même endroit ce qui abstrait le reste du programme de cette dernière (les sockets peuvent être remplacé par autre chose, ou un portage sur Windows serait plus évident).

## TODO :
- Ajouter le client
- Ajouter le multithread
- Utiliser le mécanisme d'erreur C++
- Implémenter une architecture orientée évenements
- Ajouter le vérification de connexion et les reconnexions en cas de perte
- Mécanisme de heartbeat pour éviter les deconnexion TCP

