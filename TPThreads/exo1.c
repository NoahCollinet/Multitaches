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
  int * cpt;
  pthread_mutex_t * verrou_cpt;
};


void * fonctionThread (void * params){

  // On cast params (de type void) en paramsFonctionThread
  struct paramsFonctionThread * args = (struct paramsFonctionThread *) params;
  sleep(1);
  printf("Appel depuis fonctionThread \n");
  /*
  * Les deux lignes font la même choses : 
  * (*args).idThread 
  * args->idThread
  */
  printf("Mon id est :  %i\n", (*args).idThread); 

  // Je vérouille l'accès à ma variable cpt
  pthread_mutex_lock(args->verrou_cpt);
  (*(args->cpt))++;
  pthread_mutex_unlock(args->verrou_cpt);
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
  int cpt = 0;
  pthread_mutex_t verrou_cpt;
  pthread_mutex_init(&verrou_cpt, NULL);
   
  // création des threards 
  for (int i = 0; i < atoi(argv[1]); i++){

    params[i].idThread = i;
    params[i].verrou_cpt = &verrou_cpt; // On passe le même verrou à tous les threads
    params[i].cpt = &cpt; // On passe le même commpteur
    
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

  printf("Tous tes threads ont répondu, valeur de cpt : %i\n", cpt);

  return 0;
 
}
 
