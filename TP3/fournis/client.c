#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <sys/stat.h>

#define MAX_BUFFER_SIZE 146980
//port 51510
//IP 127.0.0.1

int main(int argc, char *argv[]) {

  if (argc != 5){
    printf("utilisation : %s ip_serveur port_serveur port_client nom_fichier  \n", argv[0]);
    exit(1);
  }
  char* ip=argv[1];
  char* pServeur=argv[2];
  char* pClient=argv[3];

  /* etape 1 : créer une socket */   
   int ds = socket(PF_INET, SOCK_STREAM, 0);

   if (ds == -1){
    perror("Client : pb creation socket :");
    exit(1); 
   }

   struct sockaddr_in ad;
ad.sin_family= AF_INET;
ad.sin_addr.s_addr=INADDR_ANY;
ad.sin_port=htons(atoi(pClient));

int res=bind(ds,(struct sockaddr*)&ad,sizeof(ad));

if (res==-1)
{
   perror("Probleme du binding");
   exit(1);
}


  /* etape 2 : designer la socket du serveur */

  struct sockaddr_in adServ;
adServ.sin_family=AF_INET;
adServ.sin_addr.s_addr=inet_addr(argv[1]);
adServ.sin_port=htons((short)atoi(argv[2]));
socklen_t lgA=sizeof(struct sockaddr_in);
 
  /* etape 3 : demander une connexion */

  int res2=connect(ds,(struct sockaddr*)&adServ, lgA);
if (res2==-1)
{
   perror("Probleme du binding");
   exit(1);
}
 
  /* etape 4 : envoi de fichier : attention la question est générale.
  Il faut creuser pour définir un protocole d'échange entre le client et le serveur pour envoyer correctement un fichier */
/*
    socklen_t lgAdr=sizeof(struct sockaddr_in);
    printf("\nMessage a envoyer:\n");
    char msg[1500] ;
    scanf("%[^\n]",msg);

   int res3=send(ds,msg,sizeof(msg),0);
  
   int res1=send(ds,&msg,strlen(msg),0);

  printf("Nombre d'octets envoyes: %i", strlen(msg));
    if (res1==-1)
    {
    perror("Probleme lors de l'envois 1 du message...");
    exit(1);
    }
    viderBuffer();
    if (res3==-1)
    {
    perror("Probleme lors de l'envoi du message...");
    exit(1);
    }

*/
  int totalSent = 0;
 // variable pour compter le nombre total d'octet effectivement envoyés au serveur du début à la fin des échanges.


 
  /* le bout de code suivant est une lecture de contenu d'un fichier dont le nom est passé en paramètre.
     - pour lire un fichier, il faut l'ouvrir en mode lecture
     - la lecture se fait par blocs d'octets jusqu'à la fin du fichier.
     - la taille d'un bloc est définie par la constante MAX_BUFFER_SIZE que vous pouvez modifier.

     Le code est à compléter pour mettre en place l'envoi d'un fichier.
  */

  // construction du nom du chemin vers le fichier
  char* filepath = malloc(strlen(argv[4]) + 16); // ./emission/+nom fichier +\0
  filepath[0] = '\0';
  strcat(filepath, "../emission/");
  strcat(filepath, argv[4]);

  printf("Client: je vais envoyer %s\n", filepath);

  // obtenir la taille du fichier
  struct stat attributes;
  if(stat(filepath, &attributes) == -1){
    perror("Client : erreur stat");
    free(filepath);
    exit(1);
  }

  int file_size = attributes.st_size; // cette copie est uniquement informer d'où obtenir la taille du fichier.
  
  printf("Client : taille du fichier : %d octets\n", file_size);
  
  // lecture du contenu d'un fichier
  FILE* file = fopen(filepath, "rb");
  if(file == NULL){
    perror("Client : erreur ouverture fichier \n");
    free(filepath);
    exit(1);   
  }
  free(filepath);

  int total_lu = 0;
  char buffer[MAX_BUFFER_SIZE];
  while(total_lu < file_size){
    
    size_t read = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, file);
    if(read == 0){
      if(ferror(file) != 0){
	perror("Client : erreur lors de la lecture du fichier \n");
	fclose(file);
	exit(1);
      }else{
        printf("Client : arrivé a la fin du la lecture du fichier\n");// fin du fichier
	break;
      }
    }
    printf("Client : j'ai lu un bloc du fichier \n");  
    total_lu += read;
    buffer[total_lu]='\0';

    int res3=send(ds,buffer,total_lu,0);

  }

  // fermeture du fichier
  fclose(file); 

  printf("Client : j'ai lu au total : %d octets \n", total_lu);  

 
  printf("Client : c'est fini\n");
}

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

