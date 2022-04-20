#include <string.h>
#include <stdio.h>//perror
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
// #include <iostream>
#include <pthread.h>


// Paramètres de ma fonction threads. Pour transmettre une infromation à un thread, c'est ici !
struct paramsFonctionThread {
  int idThread;
  struct variableCommune* varCommune;
};

struct variableCommune {
    int * cpt;
    pthread_mutex_t * verrou_cpt;
    pthread_cond_t * condi;
};


void * fonctionThread (void * params){

  // On cast params (de type void) en paramsFonctionThread
  struct paramsFonctionThread * args = (struct paramsFonctionThread *) params;
    printf("je fais quelque chose\n");
  sleep(1);

  /*
  * Les deux lignes font la même choses : 
  * (*args).idThread 
  * args->idThread
  */
  printf("Mon id est :  %i\n", (*args).idThread); 

  // Je vérouille l'accès à ma variable cpt
  pthread_mutex_lock(args->varCommune->verrou_cpt);
  printf("J'ai un cpt de :%i (before)\n",*(args->varCommune->cpt));
  (*(args->varCommune->cpt))--;
  printf("Mon id est :  %i après décrémentation \n", (*args).idThread); 
  printf("J'ai un cpt de :%i (after)\n",*(args->varCommune->cpt));
  
    if(*(args->varCommune->cpt)!=0)
    {
       pthread_cond_wait(args->varCommune->condi, args->varCommune->verrou_cpt);
    }
    else
    {
        pthread_cond_broadcast(args->varCommune->condi);
    }

  pthread_mutex_unlock(args->varCommune->verrou_cpt);
    printf("On reccommence \n");

  return 0;
}


int main(int argc, char * argv[]){

  if (argc < 2 ){
    printf("utilisation: %s  nombre_threads  \n", argv[0]);
    return 1;
  }     

  // Tableau de threads
  pthread_t threads[atoi(argv[1])];

  // Tableau de paramètres
  struct paramsFonctionThread params[atoi(argv[1])];
    
  // Variables communes à tous mes threads : cpt et verrou_cpt
  struct variableCommune varCom;


  int cpt = atoi(argv[1]);
  varCom.cpt=&cpt;


  pthread_mutex_t verrou_cpt;
  pthread_mutex_init(&verrou_cpt, NULL);
  varCom.verrou_cpt = &verrou_cpt;

  pthread_cond_t condi;
  pthread_cond_init(&condi,NULL);
  varCom.condi=&condi;
   
  // création des threards 
  for (int i = 0; i < atoi(argv[1]); i++){

    params[i].idThread = i;
    params[i].varCommune = &varCom; // On passe le même verrou à tous les threads
    
    if (pthread_create(&threads[i], NULL,fonctionThread, &params[i]) != 0){
      perror("erreur creation thread");
      exit(1);
    } 
  }


  printf("creation de thread reussie\n");

  // On attend la fin des threads
  for (int i = 0; i < atoi(argv[1]); i++){
    pthread_join(threads[i],NULL);
  }

  printf("Tous les threads ont répondu, valeur de cpt : %i\n", cpt);

  return 0;
 
}
 
