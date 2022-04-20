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
  if (argc != 1){
    printf("utilisation : %s \n", argv[0]);
    exit(1);
  }
 

  /* Etape 1 : creer une socket */  
  int ds = socket(PF_INET, SOCK_STREAM, 0);

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
 ad.sin_port=htons(0);

int res=bind(ds,(struct sockaddr*)&ad,sizeof(ad));
if (res==-1)
  {
     perror("Pas de bind");
     exit(1);
  } 

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
 

while(1){
socklen_t lgA = sizeof(struct sockaddr_in);
int DSClient= accept(ds, (struct sockaddr*) &ad, &lgA);
if (DSClient==-1)
  {
     perror("Pas de connect");
     exit(1);
  } 

  /* Etape 4 : recevoir un message du client (voir sujet pour plus de details)*/
int i=0;
int appelrcv=0;
int octrecu=0;
while(i<5){

  struct sockaddr_in sockClient;
  socklen_t lgAdr;
  char messageRecu[100];
  res=recv(DSClient,messageRecu,sizeof(messageRecu),0);
appelrcv++;
octrecu +=sizeof(messageRecu);

   if (res==-1)
  {
     perror("Pas recu de message");
     exit(1);
  } 

  printf("%s\n",messageRecu);

  printf("Nombre d'octets recus: %li", octrecu);
 }

  printf("Nombre d'octets recus au total: %li en %li appels", octrecu, appelrcv); 


  /* Etape 5 : envoyer un message du serveur (voir sujet pour plus de details)*/
  
    //lgAdr=sizeof(struct sockaddr_in);
    //struct sockaddr_in sockDistante;
    printf("\nMessage a envoyer:\n");
    char msg[200] ;
    scanf("%[^\n]",msg);

    res=send(DSClient,&msg,sizeof(msg),0);
    viderBuffer();

    if (res==-1)
    {
    perror("Probleme lors de l'envoi du message...");
    exit(1);
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
  
  printf("Serveur : je termine\n");
  return 0;
}
}
int close(int DSClient); 
int close(int ds);
void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}
