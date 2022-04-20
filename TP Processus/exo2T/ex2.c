#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

int main(int argc,char* argv[]){
    if(argc!=3)
   {
       printf("Voila l'utilisation : \n");
       printf("fichier-cle-ipc entier-cle-ipc\n");
       exit(0);
   }     
   int cleSemaphore=ftok(argv[1],atoi(argv[2]));

   int idSemaphore=semget(cleSemaphore,1,0666);

   if(idSemaphore==-1)
    {
      perror("erreur semget\n");
      exit(0);
    } 


    struct sembuf OperationP;
    OperationP.sem_op=-1;
    OperationP.sem_num=0;
    OperationP.sem_flg=0;

    struct sembuf OperationZ;
    OperationZ.sem_op=0;
    OperationZ.sem_num=0;
    OperationZ.sem_flg=0;

    printf("On debute la première action\n");
    sleep(2);
    printf("On decremente\n");
    semop(idSemaphore,&OperationP,1);
    printf("On attends\n");
    semop(idSemaphore,&OperationZ,1);
    printf("C'est bon, tout le monde est la donc on peut finir ensemble!!\n");
    sleep(3);
    printf("Tout fini !\n");
    return 0;
}



