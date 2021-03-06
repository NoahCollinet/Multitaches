/*
  Programme visiteur à compléter : les zones à compléter sont indiquées et il n'est pas nécessaire d'ajouter de nouvelles traces d'exécution. 
  
  Attention : dans ce programme, il n'y a aucun besoin de simuler un temps de visite. Le temps de visite est géré par le programme bus. Aussi, les actions de monter et de descendre ne sont pas a mettre en oeuvre. La synchronisation doit assurer que le bus soit vide avant un nouvel embarquement.
  
*/


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

int main(int argc, char * argv[]){

  if (argc!=4) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s monNumero fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	  
  int cleSem=ftok(argv[2], atoi(argv[3]));

  int idSem = semget(cleSem, 1, 0666);

  // j'utilise semget de sorte a s'assurer que le tableau existe.
  if (idSem==-1){
    perror("erreur  semget");
    exit(-1);
  }

  int monNum = atoi(argv[1]);
  
  printf("Visiteur %d : Id tableau de semaphores : %d \n", monNum, idSem);
  
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
    
  printf("Visiteur %d: je demande a monter \n", monNum);
  
  semop(idSem,&OperationP,1);
    
  printf("Visiteur %d : je suis a bord et bien installe.\n", monNum); 
     
  semop(idSem,&OperationZ,1);
    
  printf("Visiteur %d : visite terminee, je descends.\n", monNum);  

  semop(idSem,&OperationV,1);
 
  return 0;
}

