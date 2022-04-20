#include <stdio.h>//perror
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>//close
#include <stdlib.h>
#include <string.h>



#define MAX_BUFFER_SIZE 146980

int main(int argc, char *argv[])
{
  /* etape 0 : gestion des paramètres si vous souhaitez en passer */
   if (argc != 1){
    printf("utilisation : %s \n", argv[0]);
    exit(1);
  }

  
  /* etape 1 : creer une socket d'écoute des demandes de connexions*/

    int ds = socket(PF_INET, SOCK_STREAM, 0);
   if (ds == -1)
   {
    perror("Serveur : pb creation socket :");
    exit(1);
   }
  
  printf("Serveur : creation de la socket reussie \n");
  /* etape 2 : nommage de la socket */

 struct sockaddr_in ad;
 ad.sin_family=AF_INET;
 ad.sin_addr.s_addr=INADDR_ANY;
 ad.sin_port=htons(0);

int res=bind(ds,(struct sockaddr*)&ad,sizeof(ad));
if (res==-1)
  {
     perror("Pas de bind");
     exit(1);
  } 


  /* etape 3 : mise en ecoute des demandes de connexions */

  int res1=listen(ds,10);
if (res1==-1)
  {
     perror("Pas de listen");
     exit(1);
  } 

  socklen_t lg = sizeof(struct sockaddr_in);
  if (getsockname(ds, (struct sockaddr*)&ad, &lg)<0){
    perror("serveur : erreur : getsockname");
    close(ds);
    exit(1);
  }
  printf("serveur : mon num de port est %d \n",ntohs(ad.sin_port));
 
  /* etape 4 : plus qu'a attendre la demande d'un client */

 while(1)
 {

 socklen_t lgA = sizeof(struct sockaddr_in);
int DSClient= accept(ds, (struct sockaddr*) &ad, &lgA);
if (DSClient==-1)
  {
     perror("Pas de connect");
     exit(1);
  } 
int totalRecv = 0;
char messageRecu[4000];
if (fork()==0)
	{
 	int res7=recv(DSClient,messageRecu,sizeof(messageRecu),0); //tcp car fork sinon rcv normal ( +1 arg 0)
   	if (res7==-1)
  		{
     		perror("Pas recu de message");
     		exit(1);
  		} 
  	printf("serveur : message : %s\n", messageRecu);
close(DSClient);
}

 
 
totalRecv +=sizeof(messageRecu); // un compteur du nombre total d'octets recus d'un client
 
  /* le protocol d'echange avec un client pour recevoir un fichier est à définir. Ici seul un exemple de code pour l'écriture dans un fichier est founi*/
   
  char* filepath = "../reception/toto.txt"; // cette ligne n'est bien-sur qu'un exemple et doit être modifiée : le nom du fichier doit être reçu.
   
  // On ouvre le fichier dans lequel on va écrire
  FILE* file = fopen(filepath, "wb");
  if(file == NULL){
    perror("Serveur : erreur ouverture fichier: \n");
    exit(1);  
  }

  /*char * buffer ="ceci est un exemple de contenu a ecrire dans un fichier\n";*/
  size_t written = fwrite(messageRecu, sizeof(char), totalRecv, file);
  if(written < totalRecv){  // cette ligne est valide uniquement pour ce simple exemple
    perror("Serveur : Erreur a l'ecriture du fichier \n");
    fclose(file); 
  }

  printf("Serveur : ecriture dans fichier reussie. Vous pouvez vérifier la création du fichier et son contenu.\n");
  // fermeture du fichier
  fclose(file);

} 

  printf("Serveur : c'est fini\n");
}








