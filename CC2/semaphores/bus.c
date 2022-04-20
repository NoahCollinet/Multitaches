/*
  Programme bus � compl�ter. Les zones � compl�ter sont indiqu�es et il n'est pas n�cessaire d'ajouter de nouvelles traces d'ex�cution.
  
  Rappel pour le d�p�t : sur Moodle, donner les instructions pour la cr�ation et l'initisalisation du tableau de semaphores n�cessaires au lancement de ./bin/semInit (voir le sujet)
*/


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include "simulation.h"

int main(int argc, char * argv[]){

  if (argc!=4) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s nombre-places fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	  
  
  int cleSem=ftok(argv[2], atoi(argv[3]));

  int idSem = semget(cleSem, 1, 0666);

  // j'utilise semget de sorte a s'assurer que le tableau existe.
  if (idSem==-1){
    perror("erreur  semget");
    exit(-1);
  }

  printf("Id tableau de semaphores : %d \n", idSem);
  
  int nbPlaces = atoi(argv[1]);


  struct sembuf OperationP;
    OperationP.sem_op=-1;
    OperationP.sem_num=0;
    OperationP.sem_flg=0;

  struct sembuf OperationV;
    OperationP.sem_op=+1;
    OperationP.sem_num=0;
    OperationP.sem_flg=0;

  struct sembuf OperationZ;
    OperationZ.sem_op=0;
    OperationZ.sem_num=0;
    OperationZ.sem_flg=0;

     
  while(1){

    // les traces d'ex�cution sont � garder inchang�es.
    
    printf("Bus : embarquement immediat, soyez les bienvenus! \n");
    
    semop(idSem,&OperationV,nbPlaces);

    printf("Bus : bus complet, la visite commence !\n"); 
    
    visite(5); // simulation de la visite. voir .h pour le param�tre
    
    printf("Bus : visite terminee, tout le monde descend !\n"); 
  
    semop(idSem,&OperationP,1);
    
    printf("Bus : attendre que le bus soit vide\n");
     
   semop(idSem,&OperationZ,1);
   
  }
  return 0;
}

