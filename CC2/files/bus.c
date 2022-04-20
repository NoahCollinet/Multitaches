/*
  Programme bus � compl�ter. Les zones � compl�ter sont indiqu�es et il n'est pas n�cessaire d'ajouter de nouvelles traces d'ex�cution.
  Vous devez expliquer en commentaires : le sens donn� au messages echang�s et aux �tiquettes.
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "simulation.h"

int main(int argc, char * argv[]){

  if (argc!=4) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s nombre-places fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	  
  
  int cle=ftok(argv[2], atoi(argv[3]));

  int idFile = msgget(cle, 0666);

  // j'utilise msgget de sorte a s'assurer que la file existe.
  if (idFile==-1){
    perror("erreur  msgget");
    exit(-1);
  }

  printf("Bus : Id File msg : %d \n", idFile);
  
  int nbPlaces = atoi(argv[1]);
  
  struct demandeMontee
      {
          long etiq; //1: demande d'acces
          int idDemandeur;
      };

      struct Autorisation
      {
          long etiq; //id demandeur
          char valeur; //Pour envoyer un truc comme ca
      };

    struct Descente
    {
        long etiq; //2: fin acces
        int idDemandeur;
    };
     
  
  while(1){
     
     // les traces d'ex�cution sont � garder inchang�es.
 
    printf("Bus : embarquement immediat, soyez les bienvenus! \n");
    
    demandeMontee demande;
    while(demande.idDemandeur!=nbPlaces){
      
    printf("Bus : attente que le bus soit complet \n");

    if(msgrcv(msgid,(void *)&demande, sizeof(int), long(1),0)==-1)
    {
        perror("Reception demande de montee ");
        exit(2);
    }
    
    Autorisation autorisation;

    autorisation.etiq=demande.idDemandeur;
    autorisation.valeur='a';

    if(msgsnd(msgid, (void *)&autorisation,sizeof(char),0)==-1)
    {
        perror("Erreur envoi autorisation:");
        exit(2);
    }

    }
    printf("Bus : bus complet, la visite commence !\n");
    
    visite(2);  // simulation de la visite. voir .h pour le param�tre
    
    printf("Bus : visite terminee, tout le monde descend !\n"); 
    
    Descente fin;

    printf("Bus : attendre que le bus soit vide\n");

    for (int i = 1; i < nbPlaces ; i++){
      
      if(msgrcv(msgid, (void *)&fin,sizeof(int),long(2),0)==-1)
      {
          perror("Reception notification\nFin d'acces");
          exit(2);
      }

    }

   
  }
  return 0;
}

