#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <stdbool.h>

//port 51510
//IP 268.38.151.21

/* Programme client */

int main(int argc, char *argv[]) {

  /* je passe en parametre l'adresse de la socket du serveur (IP et
     numero de port) et un numero de port a donner a la socket creee plus loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     parametres sont a adapter en fonction des besoins. Sans ces
     parametres, l'execution doit etre arretee, autrement, elle
     aboutira a des erreurs.*/
  if (argc != 4){
    printf("utilisation : %s ip_serveur port_serveur port_client\n", argv[0]);
    exit(1);
  }
  char* ip=argv[1];
  char* pServeur=argv[2];
  char* pClient=argv[3];

  /* Etape 1 : creer une socket */  
  int ds = socket(PF_INET, SOCK_DGRAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1){
    perror("Client : pb creation socket :");
    exit(1); // je choisis ici d'arreter le programme car le reste
         // dependent de la reussite de la creation de la socket.
  }
 
  /* J'ajoute des traces pour comprendre l'execution et savoir
     localiser des eventuelles erreurs */
  printf("Client : creation de la socket reussie \n");
 
  // Je peux tester l'execution de cette etape avant de passer a la
  // suite. Faire de meme pour la suite : n'attendez pas de tout faire
  // avant de tester.
 
  /* Etape 2 : Nommer la socket du client */

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

 /* Etape 3 : Designer la socket du serveur */
 
struct sockaddr_in sockDistante;
sockDistante.sin_family=AF_INET;
sockDistante.sin_addr.s_addr=inet_addr(ip);
sockDistante.sin_port=htons(atoi(pServeur));


printf("Fin de l'etape3");

bool test=true;

while(test)
{

  /* Etape 4 : envoyer un message au serveur  (voir sujet pour plus de details)*/
 
    socklen_t lgAdr=sizeof(struct sockaddr_in);
    printf("\nMessage a envoyer:\n");
    char msg[1500] ;
    scanf("%[^\n]",msg);

    res=sendto(ds,&msg,strlen(msg)+1,0,(struct sockaddr*)&sockDistante,lgAdr);
    res1=sendto(ds,&msg,strlen(msg),0,(struct sockaddr*)&sockDistante,lgAdr);

  printf("Nombre d'octets envoyes: %i", strlen(msg));
    if (res==-1)
    {
    perror("Probleme lors de l'envois 1 du message...");
    exit(1);
    }
    viderBuffer();
    if (res1==-1)
    {
    perror("Probleme lors de l'envois 2 du message...");
    exit(1);
    }
    viderBuffer();


  /* Etape 5 : recevoir un message du serveur (voir sujet pour plus de details)*/
 
  struct sockaddr_in sockClient;
  lgAdr;
  char message[200];
  res=recvfrom(ds,message,sizeof(message),0,&sockClient,&lgAdr);
 
  if(res==-1)
 {
    perror("Je peux pas recevoir");
    exit(1);
 }

  printf("\n%s\n");

    printf("Voulez-vous continuer de parler à ce serveur? Oui/Non");
    char reponse[3];
    scanf("[A-Z][a-z]{2}",reponse);
    if (reponse=="Non"){
        test=false;
    }
    viderBuffer();
}

  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisee)*/
 
 
  printf("Client : je termine\n");
  return 0;
}


void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}




