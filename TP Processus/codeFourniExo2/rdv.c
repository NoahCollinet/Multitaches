#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define ERROR -1

typedef int ipc_id; 

typedef union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
    struct seminfo *__buf;
} SemUn;

typedef struct sembuf SemBuf;



int main(int argc, char * argv[]){
    if (argc != 5) {
        printf("Utilisation: %s nombreRdv nombreProcessus fichierCle entierCle\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Création de la sémaphore.
    int nbSem = atoi(argv[1]);

    key_t clesem = ftok(argv[3], atoi(argv[4]));
    ipc_id idSem = semget(clesem, nbSem, IPC_CREAT|IPC_EXCL|0600);
    
    if(idSem == ERROR){
        perror("Erreur lors du semget ");
        exit(EXIT_FAILURE);
    }

    printf("ID de la sémaphore : %d\n", idSem);

    // Initialisation des sémaphores 
    ushort tabinit[nbSem];
    for (int i = 0; i < nbSem; ++i) 
        tabinit[i] = atoi(argv[2]);

    SemUn egCtrl;
    egCtrl.array = tabinit;

    if (semctl(idSem, 0, SETALL, egCtrl) == ERROR) {
        perror("Erreur lors de l'initialisation des sémaphores ");
    }
    
    egCtrl.array = (ushort*)malloc(nbSem * sizeof(ushort)); 

    if (semctl(idSem, nbSem, GETALL, egCtrl) == ERROR) {
        perror("Erreur d'initialisation des sémaphores ");
    }
    
    printf("Valeurs des sempahores apres initialisation [ "); 
    for(int i = 0; i < nbSem - 1; i++) {
        printf("%d, ", egCtrl.array[i]);
    } 
    printf("%d ] \n", egCtrl.array[nbSem - 1]);

    //Operation P
    struct sembuf opp;
    opp.sem_num = 0;
    opp.sem_op = -1;
    opp.sem_flg = 0;
    semop(idSem, &opp, 1);

    //Operation V
    struct sembuf opv;
    opv.sem_num = 0;
    opv.sem_op =+1;
    opv.sem_flg = 0;
    semop(idSem, &opv, 1);


    // Attente des enfants 
    while (wait(NULL) > 0) ;

    free(egCtrl.array);
    if (semctl(idSem, 0, IPC_RMID) == ERROR) {
        perror("Erreur lors de la fin du processus ");
    }
    return 0;
}