/*
  Programme visiteur à compléter : les zones à compléter sont indiquées et il n'est pas nécessaire d'ajouter de nouvelles traces d'exécution. 
  
 Vous devez expliquer en commentaires : le sens donné au messages echangés et aux étiquettes.
 
  Attention : dans ce programme, il n'y a aucun besoin de simuler un temps de visite. Le temps de visite est géré par le programme bus.
   
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
    printf("%s monNumero fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	 
  initDefault(50); // ne pas modifier ni déplacer cet appel.
  
  int cle =ftok(argv[2], atoi(argv[3]));

  int idFile = msgget(cle, 0666);

  // j'utilise msgget de sorte a s'assurer que la file existe.
  if (idFile == -1){
    perror("erreur  msgget");
    exit(-1);
  }

  int monNum = atoi(argv[1]);
  
  printf("Visiteur %d : Id file msg : %d \n", monNum, idFile);
  
  struct demandeMontee
  {
    long etiq;
    int id;
  };

  struct Autorisation
  {
    long etiq;
    char valeur;
  };

  struct Descente
  {
    long etiq;
    int id;
  };
  int getid=getpid();
  while(1)
  {
    demandeMontee demande;
    demande.etiq=1;
    demande.id=getid;

    printf("Visiteur %d: je demande a monter \n", monNum);
    if(msgsnd(msgid,(void *)&demande,sizeof(int),0)==-1)
    {
      perror("Erreur lors de la demande de montee \n");
      exit(1);
    }
    cout<<"Demande de montee reussie\n"<<endl;

    Autorisation autorisation;
    if(msgrcv(msgid,(void *)&autorisation,sizeof(char),long(getpid()),0)==-1)
      {
        perror("Erreur lors de la reception");
        exit(2);
      }
    
    printf("Visiteur %d : je monte ...\n", monNum);
    monterOuDescendre();

    printf("Visiteur %d : je suis a bord et bien installe\n", monNum);
      
    
  Descente fin;
  fin.etiq=2;
  fin.id=getid;

    printf("Visiteur %d : visite terminee, je descends ...\n", monNum);
    monterOuDescendre();

    if(msgsnd(msgid,(void *)&fin,sizeof(int),0)==-1)
      {
        perror("Erreur lors de l'envois\n");
        exit(2);
      }

    
    printf("Visiteur %d : je suis descendu.\n", monNum);  
  
  }
  return 0;
  
}

