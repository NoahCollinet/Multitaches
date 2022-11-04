#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

#define ERROR -1
int recvTCP(int sock, char * msg, int sizeMsg)
{
  //--Enlever si Serveur concurent sinon il est trop cool
  getchar();

  int verif=0;
  while(verif < sizeMsg)
    {
      int rest=recv(sock, msg, sizeMsg,0);
      printf("rest %i\n",rest);
      verif=verif+(int)rest;
      printf("verif= %d, sizeMsg = %d \n",verif,sizeMsg);
      if (rest == -1)
      {
      perror("Serveur : Je ne recois pas de message \n");
      exit(1);
      }
    
      printf("Serveur : Message reçu : \n%s\n",msg);
      printf("Nombre d'octets reçus : %d \n",(int)rest);

    } 
 
  return 1;
}



int sendTCP(int sock, char * msg, int sizeMsg)
{
  int verif=0;
  int cpt=0;
while(verif<sizeMsg)
        {
          printf("Passage \n");
          fgets(msg,1500,stdin);
          msg[strlen(msg)] = NULL;
          printf("==> %s \n",msg);
          int res = send(sock, msg, strlen(msg),0);
          cpt=cpt+(int)res;
          verif=verif+(int)res;
          printf("verif= %d, sizeMsg = %d \n",verif,sizeMsg);
          if(res == -1)
	        { printf("Client : Le message n'est pas parti T-T\n");}
          
        }
        printf("Nombre d'octets envoyés : %d \n",cpt);
      if(verif==sizeMsg)
      {return 1;}
      
}

/* Programme serveur */

int main(int argc, char *argv[]) {


  if (argc != 1){
    printf("utilisation : %s \n", argv[0]);
    exit(1);
  }

   /* Etape 1 : Création de la socket du seveur */
 
  int ds = socket(PF_INET, SOCK_STREAM, 0);


  if (ds == -1){
    perror("Serveur : probleme creation socket :");
    exit(1); 
  }
  else{printf("Serveur : creation de la socket réussie \n");}
  
  // Je peux tester l'exécution de cette étape avant de passer à la
  // suite. Faire de même pour la suite : n'attendez pas de tout faire
  // avant de tester.
  
  /* Etape 2 : Nommer la socket du seveur */
  struct sockaddr_in ads;
  ads.sin_family = AF_INET;
  ads.sin_addr.s_addr = INADDR_ANY;
  ads.sin_port = htons(0);//(short) atoi(argv[1])

  int res = bind(ds, (struct sockaddr*)&ads, sizeof(ads));

  if(res == 0)
    {
      printf("Serveur : Nommage fait, numéro de port du socket: %d\n", ntohs(ads.sin_port));
    }
  else
    { printf("Serveur : Erreur de nommage, on recommence\n");}

  printf("Serveur : En attente\n");


  int lt= listen(ds,10);

 socklen_t lg = sizeof(struct sockaddr_in);

  if (getsockname(ds, (struct sockaddr*)&ad, &lg)<0){
    perror("serveur : erreur : getsockname");
    close(ds);
    exit(1);
  }
 
 

//Serveur version itérative--------------
/*
  while(1){

  struct sockaddr_in sockClient;
  socklen_t lgAdr; // sizeof(sockaddr_in)
  
  int dsClient = accept(ds,(struct sockaddr*)&sockClient,&lgAdr);

  getchar();
  char message[4000];
  //ssize_t rest = recv(dsClient, message, sizeof(message),0);
  printf("Serveur : message reçu de : %s:%d \n", inet_ntoa(sockClient.sin_addr), ntohs(sockClient.sin_port));
  int res5 = recvTCP(dsClient, message,50);
      
  char msg[200]= "Message du client au serveur \n";
  int res2 = send(dsClient, msg, strlen(msg)+1,0);
  //int res2 = sendTCP(dsClient, msg,200);

  close(dsClient);

}
*/
  
//------------------------------------

//Serveur version Concurente

 while (1)
    {
        struct sockaddr_in client;
        socklen_t adresse = sizeof(struct sockaddr_in);
        int dSClient = accept(ds,(struct sockaddr*)&client,&adresse);
        if (dSClient < 0)
        {
            perror("Serveur : erreur dans accept : ");
            close(ds);
            exit(1);
        }
        else
        {
            printf("Serveur : accept réussi \n");
            printf("Serveur: le client %s:%d est connecte  \n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        }

        //comment faire un fork ...
        int parent = getpid();
        printf("Avant fork : pid parent racine : %d \n", parent);
        int resFork = fork();
        if (resFork == -1)
        {
        perror("Erreur fork : ");
        close(dSClient);
        exit(1);
        }
        
        //recevoir la taille du message
        if (resFork==0){
        char message[4000];
        int recu=recvTCP(dSClient,message,50);
        printf("dsClient : %i\n taille message : %i\n",dSClient,sizeof(message));
        if (recu < 0)
        {
            perror("Serveur : probleme reception :");
            close(dSClient);
            exit(1);
        }

        if (recu == 0)
        {
            printf("Serveur : client deconnecte: \n");
            close(dSClient);
            exit(1);
        }
	printf("Serveur: Message recu!\n");
        ssize_t res2 = recvTCP(dSClient,message,50);
        if (res2 < 0)
        {
            perror("Serveur : erreur dans rcv : ");
            close(dSClient);
            exit(1);
        }


        if (res2 == 0)
        {
            printf("Serveur : client deconnecte: \n");
            close(dSClient);
            exit(1);
        }

        printf("%s\n",message);
        printf("Serveur : nombre d'octets reçus : %d \n", (int)res2);

        printf("Processus : %d terminé \n", getpid());
        exit(0);
        }
    }



//-----------------------



 
  /*if( rest == -1)
    {printf("Serveur : Impossible de recevoir\n");}*/
  //printf("Nombre d'octets reçu : %d \n",(int)rest);  
  //printf("Serveur : Message reçu : \n%s",message);

  /* Etape 5 : envoyer un message au serveur (voir sujet pour plus de détails)*/
  
  

 /* if(res2 == -1)
	{ printf("Serveur : Le pigeon n'est pas parti :(\n");
  return 1;}

  printf("Serveur: Réponse client : %s \n", msg);*/

  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  close(ds);
  printf("Serveur : je termine\n");
  return 0;
}
