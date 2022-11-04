#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

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
	        { printf("Client : Le message n'est pas parti :(\n");}
          
        }
        printf("Nombre d'octets envoyé : %d \n",cpt);
      if(verif==sizeMsg)
      {return 1;}
      
}


/* Programme client */

int main(int argc, char *argv[]) {
  // IP du td: 127.0.0.0
  /* je passe en paramètre l'adresse de la socket du serveur (IP et
     numéro de port) et un numéro de port à donner à la socket créée plus loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     paramètres sont à adapter en fonction des besoins. Sans ces
     paramètres, l'exécution doit être arrétée, autrement, elle
     aboutira à des erreurs.*/
  if (argc != 5){
    printf("utilisation : %s ip_serveur port_serveur port_client nb itérations\n", argv[0]);
    exit(1);
  }


  /* Etape 1 : créer une socket */   
  int ds = socket(PF_INET, SOCK_STREAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1){
    perror("Client : probleme creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
	     // dépendent de la réussite de la création de la socket.
  }
  
  /* J'ajoute des traces pour comprendre l'exécution et savoir
     localiser des éventuelles erreurs */
  printf("Client : creation de la socket réussie \n");
  
  // Je peux tester l'exécution de cette étape avant de passer à la
  // suite. Faire de même pour la suite : n'attendez pas de tout faire
  // avant de tester.
  
  /* Etape 2 : Nommer la socket du client */
  /*  int bind (int descript, const struct sockaddr_in *Adresse 

      Adresse{AP_INET, argv[2] + argv[4]};*/

  struct sockaddr_in ad;
  ad.sin_family = AF_INET;
  ad.sin_addr.s_addr = INADDR_ANY;
  ad.sin_port = htons((short) atoi(argv[3]));

  int res = bind(ds, (struct sockaddr*)&ad, sizeof(ad));

  if(res == 0)
    {
      printf("Client : Nommage ok \n");
    }
  else
    { printf("Client : Erreur, on recommence\n");}
      
  /* Etape 3 : Désigner la socket du serveur */
      struct sockaddr_in sockDistante;
      sockDistante.sin_family =AF_INET;
      sockDistante.sin_addr.s_addr = inet_addr(argv[1]);
      sockDistante.sin_port = htons( (short) atoi(argv[2]));

  socklen_t lgAdr = sizeof(struct sockaddr_in);

  int c= connect(ds,(struct sockaddr*)&sockDistante,lgAdr);


  /* Etape 4 : envoyer un message au serveur  (voir sujet pour plus de détails)*/
      printf("Client : Ecrivez votre message..  \n");
      //char msg[200]="Message de moi a moi \n";
      char msg[50];
      //fgets(msg,1500,stdin);

    int res5= sendTCP(ds,msg,50);
    if(res5 == 1)
    {printf("Client : Message envoyé ! \n");}

      /*
      printf("Client : Ecrivez votre deuxième message.. MAINTENANT ! \n");
      char msg2[1500];
      fgets(msg2,1500,stdin);
      msg[strlen(msg)] = NULL;
      */
      //msg[strlen(msg)+1] = ' ';
      
      //msg2[1499] = NULL;
      //scanf("%s",msg);


    /*
      int cpt=0;
      int stop=htons(atoi(argv[4]));
      for(int i=0; i<stop; i++)
        {
          //printf("Passage \n");
          res = send(ds, msg, strlen(msg),0);
          cpt=cpt+(int)res;
          if(res == -1)
	        { printf("Client : Le pigeon n'est pas parti :(\n");}

        }
      */

      /*
      res = send(ds, msg, strlen(msg),0);
      int res2 = send(ds, msg2, strlen(msg2),0);
      

      if(res == -1 || res2 == -1)
	    { printf("Client : Le pigeon n'est pas parti :(\n");}
   
      printf("Nombre d'octets envoyé : %d \n",(int)res + (int)res2); 
      */

      

      //printf("Nombre d'octets envoyé : %d \n",cpt);

      






      
  /*Etape 5 : recevoir un message du serveur (voir sujet pour plus de détails)*/
  //NULL/NULL
 char message[200];
  res = recvfrom(ds, &message, sizeof(message),0,NULL, NULL);
  if(res < 0)
  {
    printf("Client : Message non reçu du serveur \n");
    exit(1);
  }
   
  printf("%s\n",message);
  
  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  
  close(ds);
  printf("Client : je termine\n");
  return 0;
}
