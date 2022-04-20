#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

int main(int argc, char **argv) {
    // Un client se connecte automatiquement a un port.
    if (argc != 4) {
        printf("[CLIENT] Utilisation : %s ip_serveur port_serveur port_connexion\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Création de la socket client qui va se connecter avec le serveur.
    int clientSocket = createTCPSocket();

    // Connexion avec la socket du serveur, désignée par l'ip et le port donné en paramètre du programme.
    struct sockaddr_in serverAddress = createIPv4Address(argv[1], (short)atoi(argv[2]));
    connectTo(clientSocket, &serverAddress);

    printf("[CLIENT] Connecté au serveur %s:%i.\n\n", 
        inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));

    char receivedMessage[MAX_BUFFER_SIZE];

    // Étape 1
    receiveTCP(clientSocket, receivedMessage);
    printf("%s\n", receivedMessage);
    sendTCP(clientSocket, receivedMessage, strlen(receivedMessage) + 1);

    // Étape 2
    receiveTCP(clientSocket, receivedMessage);
    printf("\n%s\n", receivedMessage);

    short servPort = (short)atoi(argv[3]);

    // Création d'une connexion serveur
    int serverSocket = createTCPSocket();
    struct sockaddr_in serverSockAddr = createIPv4Address("", servPort);
    bindSocket(serverSocket, &serverSockAddr);
    printf("[SERVEUR] Le serveur s'exécute sur %s:%i.\n", 
        inet_ntoa(serverSockAddr.sin_addr), ntohs(serverSockAddr.sin_port));
    if (listen(serverSocket, 10) == ERROR) {
        perror("[SERVEUR] Erreur lors de la mise en écoute de la socket ");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        sleep(2);
        send(clientSocket, &servPort, sizeof(short), 0);
        printf("[LOG] Envoie au serveur du port client réussi.\n");
        return 0;
    } 
    else {
        struct sockaddr_in clientSockAddr;
        int servClientSocket = acceptConnection(serverSocket, &clientSockAddr);

        printf("[LOG] Client connecté : %s:%i\n", inet_ntoa(clientSockAddr.sin_addr), ntohs(clientSockAddr.sin_port));

        //close(serverSocket);

        // Étape 3
        receiveTCP(servClientSocket, receivedMessage);
        printf("\n%s\n", receivedMessage);
    }

    return 0;
}