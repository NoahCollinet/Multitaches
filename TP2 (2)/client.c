#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <stdbool.h>
#include<errno.h>
//port 51510
//IP 127.0.0.1

/* Programme client */

void afficheErreur() {
    printf("errno: %d , %s\n", errno, strerror(errno));
}

void quitterFork(int sock) {
    close(sock);
    exit(1);
}

int sendTCP(int sock, void* msg, int sizeMsg) {
    int res;
    int sent=0;
    //boucle d'envoi
    while(sent < sizeMsg) {
        //printf("Reste à envoyer: %i (res %i) envoyé %i\n",sizeMsg-sent, res, sent);
        res = send(sock, msg+sent, sizeMsg-sent, 0);
        sent += res;
        if (res == -1) {
            printf("Problème lors de l'envoi du message\n");
            afficheErreur();
            quitterFork(sock);
            return -1;
        }
        if (res == 0) {
            printf("Déconnexion\n");
            quitterFork(sock);
            return 0;
        }
    }
    return sent;
}

int main(int argc, char *argv[]) {

  /* je passe en parametre l'adresse de la socket du serveur (IP et
     numero de port) et un numero de port a donner a la socket creee plus loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     parametres sont a adapter en fonction des besoins. Sans ces
     parametres, l'execution doit etre arretee, autrement, elle
     aboutira a des erreurs.*/
  if (argc != 5){
    printf("utilisation : %s ip_serveur port_serveur port_client nbr_iterations\n", argv[0]);
    exit(1);
  }
  char* ip=argv[1];
  char* pServeur=argv[2];
  char* pClient=argv[3];

  /* Etape 1 : creer une socket */  
  int ds = socket(PF_INET, SOCK_STREAM, 0);

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
 
struct sockaddr_in adServ;
adServ.sin_family=AF_INET;
adServ.sin_addr.s_addr=inet_addr(argv[1]);
adServ.sin_port=htons((short)atoi(argv[2]));
socklen_t lgA=sizeof(struct sockaddr_in);

int res2=connect(ds,(struct sockaddr*)&adServ, lgA);
if (res2==-1)
{
   perror("Probleme du binding");
   exit(1);
}

printf("Fin de l'etape3");
 

  /* Etape 4 : envoyer un message au serveur  (voir sujet pour plus de details)*/
 
    socklen_t lgAdr=sizeof(struct sockaddr_in);
    printf("\nMessage a envoyer:\n");
int i=0;
int appelsend=0;
int octenv=0;
    char msg[50] ;
    scanf("%[^\n]",msg);
while (i<argv[4]) {
   int res3=send(ds,msg,sizeof(msg),0);
	appelsend++;
	i++;
	octenv +=sizeof(msg);
  
   /*int res1=send(ds,&msg,strlen(msg),0);

  printf("Nombre d'octets envoyes: %i", strlen(msg));
    if (res1==-1)
    {
    perror("Probleme lors de l'envois 1 du message...");
    exit(1);
    }
    viderBuffer();*/
    if (res3==-1)
    {
    perror("Probleme lors de l'envoi du message...");
    exit(1);
    }
}

printf("\n Total d'envois en octet : %i, en %i appels à send. \n", octenv, appelsend);

  /* Etape 5 : recevoir un message du serveur (voir sujet pour plus de details)*/
 
  struct sockaddr_in sockClient;
  char message[200];
  res=recv(ds,message,sizeof(message),0);
 
  if(res==-1)
 {
    perror("Je peux pas recevoir");
    exit(1);
 }

  printf("\n %s \n",message);

    /*printf("Voulez-vous continuer de parler à ce serveur? Oui/Non");
    char reponse[3];
    scanf("[A-Z][a-z]{2}",reponse);
    if (reponse=="Non"){
        test=false;
    }
    viderBuffer();*/


  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisee)*/
 
 
  printf("Client : je termine\n");
  return 0;

int close(int ds);

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

}
