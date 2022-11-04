#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <stdbool.h>

/* Ce programme est à compléter et à déposer sur Moodle (c'est le seul fichier à déposer).

   Lire attentivement les instructions en commentaires pour compléter correctement ce programme.
 
   Les principales étapes de ce programme sont :
 
   1) demander à un serveur UDP une liste d'adresses de serveurs TCP existants.
 
   2) se connecter à l'un de ces serveurs TCP et échanger avec ce serveur.
 
   3) faire évoluer votre programme pour ajouter un serveur TCP qui échangera avec un exécutable rni.
 
   4) modifier votre programme pour qu'il soit capable de traiter plusieurs clients de manière itérative.
 
   5) modifier votre programme pour qu'il soit capable de traiter plusieurs clients  simultanéments (en utilisant la fonction fork()).
 
   Attention : vous devez déposer un code qui compile. Exemple : si vous êtes à l'étape 4 qui n'est pas fonctionnelle, la mettre en commentaire pour que toutes les étapes d'avant soient validées.
  
*/

#define MAXSRV 100

int recvTCP(int sock, char * msg, int sizeMsg)
{
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
	        { printf("Client : Le message n'est pas parti \n");}
          
        }
        printf("Nombre d'octets envoyés : %d \n",cpt);
      if(verif==sizeMsg)
      {return 1;}
      
}


// cette structure est le type du message à envoyer au serveur UDP
struct nomClient{
  short type; // doit être égal à 1
  char hostname[50];
};

typedef struct nomClient;

int main(int argc, char *argv[]) {

  if (argc != 4){
    printf("Utilisation : %s ip_serveurUDP port_serveurUDP param3 \n param3 est un entier dont la signification sera fournie par une question. En attendant cette question, passer n'importe quelle valeur\n", argv[0]);
    exit(0);
  }
  
  /* Etape 1 : envoyer un message au serveur UDP et recevoir une réponse :
  
     1) le message à envoyer est de type sNomClient. Il permet d'envoyer le nom de la machine sur laquelle s'exécute votre client. Voir plus loin le code fourni pour vous aider.
   
     2) le message à recevoir est une liste d'élements de type sock_addr_in. Chaque élément représente l'adresse d'un serveur TCP existant.
   
  */
  
  int dSUDP= socket(PF_INET, SOCK_DGRAM, 0);
  
  if (dSUDP < 0){
    perror("Erreur a la creation de la socket :");
    return 1;
  }     
 
  // designer la socket distante du serveur UDP 
  
  struct sockaddr_in aD; 
                
  aD.sin_family = AF_INET; 
  aD.sin_addr.s_addr = INADDR_ANY;
  aD.sin_port = htons(0);

  int res = bind(dSUDP, (struct sockaddr*)&aD, sizeof(aD));

  if(res == 0)
    {
      printf("Nommage de la socket distante effectué \n");
    }
  else
    { printf("Erreur du nommage de la socket distante\n");}


 socklen_t lg = sizeof(struct sockaddr_in);

  if (getsockname(dSUDP, (struct sockaddr*)&aD, &lg)<0){
    perror("serveur : erreur : getsockname");
    close(dSUDP);
    exit(1);
  }


  // Le code suivant prépare le message à envoyer au serveur UDP.
  sNom;
  nom.type = 1;
  nom.hostname[0]='\0';
  char h[50];
  gethostname(h, 50); // récupère le nom de la machine que vous utilisez.
  strcat(nom.hostname, h);
  // Le message à envoyer au serveur UDP est construit.
  // Envoyer ce message au serveur UDP
 
  int res2=sendto(dSUDP,nom,strlen(nom)+1,0,(struct sockaddr*)&aD,lg);

  // la réponse à recevoir est a stocker dans le tableau suivant. votre programme recevra au plus 50 adresses de serveurs TCP
  struct sockaddr_in reponse[50];
  int res3=recvfrom(dSUDP,reponse,sizeof(reponse),0,NULL,NULL);
 

  /* affichage de la liste des adresses recues et demande de choix d'un
     serveur. On suppose qu'au moins un serveur tourne. En l'absence d'un serveur, on termine proprement */

  printf("Les serveurs TCP existants : \n"); 
  for(int i = 0; i < sizeof(reponse)+1; i++) // Attention : une modification doit être faite ici.
    printf("%d. IP %s, port %d \n", i+1, inet_ntoa(reponse[i].sin_addr),  ntohs(reponse[i].sin_port));
    
  printf("Choisir un numero de serveur (1 ou 2 ou ... ): \n");
  int numS;
  scanf("%d", &numS);

  
  // A présent, plus besoin d'échanger avec le serveur UDP.
 
 
  /* Etape 2 : Echanger avec le serveur TCP choisi. 
    
     Après connexion :
 
     1) recevoir une chaîne de caractères dont la taille est connue par le serveur (à vous d'en déduire ce qui est nécecessaire de faire) puis afficher le message reçu.
   
     2) recevoir un entier. 
     
     Remarque : le serveur peut mettre fin à l'échange avec votre 'importe quel moment. Il faut donc bien prévoir ce cas.
   
     3) termine les échanges avec le serveur TCP.
      
  */ 
  
 
  int ds = socket(PF_INET, SOCK_DGRAM, 0);
  if (ds == -1){
    perror("probleme de la creation socket etape 2:");
    exit(1);
  }
  printf("creation de la socket reussie etape 2 \n");

struct sockaddr_in ad;
ad.sin_family= AF_INET;
ad.sin_addr.s_addr=INADDR_ANY;
ad.sin_port=htons(atoi(0));

int res4=bind(ds,(struct sockaddr*)&ad,sizeof(ad));

if (res4==-1)
{
   perror("Probleme du binding");
   exit(1);
}
int lt= listen(ds,10);

struct sockaddr_in sockClient;
  socklen_t lgAdr;
  char message[200];
  int res5=recvfrom(ds,message,sizeof(message),0,&sockClient,&lgAdr);
 
  if(res5==-1)
 {
    perror("Je peux pas recevoir");
    exit(1);
 }
printf("%s\n",message);
printf("Nombre d'octets envoyes: %li", sizeof(message));

printf("Je termine avec le serveur TCP\n");
return 0;
close(ds);  


  // L'étape suivante est indépendante de la précédente. Donc, elle peut s'exécuter même en cas d'erreurs / fermetures de scokets lors des précédents échange.
  
  /* Etape 3 : Mettre en place un serveur TCP. Ensuite passer à l'étape 4 : Modifier ce serveur pour qu'il soit itératif. Enfin, l'étape 5 : modifier le serveur pour qu'il 
  soit concurrent (en utilisant la fonction fork()). 
 
     Pour l'étape 3, le serveur a un numéro de port qui est le dernier paramètre de votre programme. Ce serveur doit faire ce qui suit pour chaque 
     1) recevoir un tableau de 320000 entiers (int) envoyé par ce client
     2) renvoyer le tableau reçu au client.
     3) recevoir en réponse une chaine de caractères dont la taille est connue par le vous de savoir ce qu'il est nécessaire de faire pour l'obtenir) 
     4) terminer l'échange avec ce client
     
     Pour tester votre serveur, executez le le binaire ./bin/clients fourni. Ce dernier lancera 5 clients.
  */ 
  
 // A vous de jouer.
    
//serveur itératif

while(1){

  struct sockaddr_in sockClient;
  socklen_t lgAdr; // sizeof(sockaddr_in)
  
  int dsClient = accept(ds,(struct sockaddr*)&sockClient,&lgAdr);

  getchar();
  int message[320000];
  ssize_t rest = recv(dsClient, message, sizeof(message),0);
  printf("Serveur : message reçu de : %s:%d \n", inet_ntoa(sockClient.sin_addr), ntohs(sockClient.sin_port));
  int res6 = recvTCP(dsClient, message,50);
      
  char msg[200]= "Message du client au serveur \n";
  int res7 = send(dsClient, msg, strlen(msg)+1,0);
  int res8 = sendTCP(dsClient, message,sizeof(message));
  viderBuffer();

  close(dsClient);

}


//serveur concurrent
 while (1)
    {
        struct sockaddr_in client;
        socklen_t adresse = sizeof(struct sockaddr_in);
        int dSClient = accept(ds,(struct sockaddr*)&client,&adresse);
        if (dSClient < 0)
        {
            perror("erreur dans accept : ");
            close(ds);
            exit(1);
        }
        else
        {
            printf("accept réussi \n");
            printf(" le client %s:%d est connecte  \n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        }

        int parent = getpid();
        printf("Avant fork : pid parent racine : %d \n", parent);
        int resFork = fork();
        if (resFork == -1)
        {
        perror("Erreur fork : ");
        close(dSClient);
        exit(1);
        }
        
        if (resFork==0){
        int message[320000];
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
	printf("Serveur: Message recu\n");
        ssize_t res9 = recvTCP(dSClient,message,50);
        if (res9 < 0)
        {
            perror("Serveur : erreur dans le recvTPC : ");
            close(dSClient);
            exit(1);
        }


        if (res9 == 0)
        {
            printf("Serveur : client deconnecte: \n");
            close(dSClient);
            exit(1);
        }

        printf("%s\n",message);
        printf("Serveur : nombre d'octets reçus : %d \n", (int)res9);

        printf("Processus : %d terminé \n", getpid());
        exit(0);
        }
    }





}


void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
} 
