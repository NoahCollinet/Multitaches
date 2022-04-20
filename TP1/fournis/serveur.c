#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <stdbool.h>

/* Programme serveur */

int main(int argc, char *argv[]) {

  /* Je passe en parametre le numero de port qui sera donne a la socket creee plus loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     parametres sont a adapter en fonction des besoins. Sans ces
     parametres, l'execution doit etre arretee, autrement, elle
     aboutira a des erreurs.*/
  if (argc != 2){
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }
  char* pServeur=argv[1];

  /* Etape 1 : creer une socket */  
  int ds = socket(PF_INET, SOCK_DGRAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1){
    perror("Serveur : pb creation socket :");
    exit(1); // je choisis ici d'arreter le programme car le reste
         // dependent de la reussite de la creation de la socket.
  }
 
  /* J'ajoute des traces pour comprendre l'execution et savoir
     localiser des eventuelles erreurs */
  printf("Serveur : creation de la socket reussie \n");
 
  // Je peux tester l'execution de cette etape avant de passer a la
  // suite. Faire de meme pour la suite : n'attendez pas de tout faire
  // avant de tester.
 
  /* Etape 2 : Nommer la socket du seveur */

 struct sockaddr_in ad;
 ad.sin_family=AF_INET;
 ad.sin_addr.s_addr=INADDR_ANY;
 ad.sin_port=htons(atoi(pServeur));

 int res=bind(ds,(struct sockaddr*)&ad,sizeof(ad));
bool test=true;
char* reponse;
while (test)
{

  /* Etape 4 : recevoir un message du client (voir sujet pour plus de details)*/
  struct sockaddr_in sockClient;
  socklen_t lgAdr;
  char messageRecu[4000];
  res=recvfrom(ds,messageRecu,sizeof(messageRecu),0,&sockClient,&lgAdr);

   if (res==-1)
  {
     perror("Pas recu de message");
     exit(1);
  } 
  printf("%s\n",messageRecu);
  printf("Nombre d'octets envoyes: %i", strlen(msg));

    
  /* Etape 5 : envoyer un message du serveur (voir sujet pour plus de details)*/
  
    lgAdr=sizeof(struct sockaddr_in);
    //struct sockaddr_in sockDistante;
    printf("\nMessage a envoyer:\n");
    char msg[200] ;
    scanf("%[^\n]",msg);

    res=sendto(ds,&msg,strlen(msg)+1,0,(struct sockaddr*)&sockClient,lgAdr);
    viderBuffer();

    if (res==-1)
    {
    perror("Probleme lors de l'envois du message...");
    exit(1);
    }

}   
    /*
  // Etape 6 : recevoir un message du serveur (voir sujet pour plus de details)
  
  struct sockaddr_in sockClient1;
  lgAdr;
  char messageRecu[200];
  res=recvfrom(ds,messageRecu,sizeof(messageRecu),0,&sockClient1,&lgAdr);
  
  if(res==-1)
 {
    perror("Je peux pas recevoir");
    exit(1);
 } 

  printf("\n%s\n");
  printf("\n%s\n");
  */

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