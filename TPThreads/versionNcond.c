#include <string.h>
#include <stdio.h>//perror
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>


// Paramètres de ma fonction threads. Pour transmettre une infromation à un thread, c'est ici !
struct paramsFonctionThread {
  int idThread;
  struct variablesCommunes* varCommunes;
};

// NbZone et verrou n'ont pas besoin d'être des pointeurs
// En effet, on déclare une seule instance de la structure varCommunes dans le main et c'est à elle
// que l'on assigne un entier pour nbZone et un verrou à verrou
// Dans variable commune, les seuls pointeurs dont vous avez besoin sont pour faire des tableaux (en général)
// Ici par exemple, di et cond dont des tableaux
struct variablesCommunes {
    int nbZone;
    int * di;
    pthread_mutex_t verrou;
    pthread_cond_t * cond;
};


void * traitement (void * p) {

  struct paramsFonctionThread * args = (struct paramsFonctionThread *) p;
  // La line suivante permet de renomer args -> varCommunes en varCommunes. 
  // Il s'agit d'un renomage pour simplifier la lisibilité du code, mais ce n'est pas obligatoire
  struct  variablesCommunes *  varCommunes = args -> varCommunes;

  // Je regarde si je peux traiter la zone que je veux traiter (la zone i)
  for(int i = 0; i <= varCommunes->nbZone; i++){
      printf("[%i] Je veux traiter la zone: %i \n",args->idThread, i);

      pthread_mutex_lock(&(varCommunes->verrou));
      if( args -> idThread != 0){ // le premier traitement n'attend personne
      // Si la zone n'est pas disponible, je me mets en attente
        if(varCommunes->di[args->idThread-1]<=i)
        {
          printf("[%i] Mise en attente : j'attends pour traiter la zone : %i \n",args->idThread, i);
          pthread_cond_wait(&(varCommunes->cond[args->idThread-1]), &(varCommunes->verrou));
        }
      }
      pthread_mutex_unlock(&(varCommunes->verrou));

      printf("[%i] Je traite la zone: %i \n",args->idThread, i);
      sleep(1); // Ou calcul()

      // Mise à jour du tableau à la fin du traitement
      pthread_mutex_lock(&(varCommunes->verrou));
      varCommunes->di[args->idThread]++;
      pthread_mutex_unlock(&(varCommunes->verrou));

      // Envoi d'un message pour signaler que la tableau a été mis à jour
      printf("[%i] Envoie d'une notification de mise à jour du tableau\n",args->idThread);
      pthread_cond_signal(&(varCommunes->cond[args->idThread]));
  }

 
  pthread_exit(NULL); 
}




int main(int argc, char * argv[]){
  
  if (argc!=3) {
    std::cout << " argument requis " << std::endl;
    std::cout << "./prog nombre_Traitements nombre_Zones" << std::endl;
    exit(1);
  }

 
   // initialisations 
  pthread_t threads[atoi(argv[1])];
  struct paramsFonctionThread tabParams[atoi(argv[1])];
  struct variablesCommunes varCommunes;

  // NbZones
  varCommunes.nbZone = atoi(argv[2]);

  // Verrou
  pthread_mutex_t verrou;
  pthread_mutex_init(&verrou, NULL);
  varCommunes.verrou = verrou;
  // Ou tout simplement 
  // pthread_mutex_init(&(varCommunes.verrou), NULL);

  // Di 
  varCommunes.di = (int *)malloc(sizeof(int) * atoi(argv[1]));
  for (int i = 0; i <atoi(argv[1]); i++) {
    varCommunes.di[i] = 0;
  }

  // cond 
  varCommunes.cond=(pthread_cond_t*)malloc(sizeof(pthread_cond_t)*atoi(argv[1])-1);
  for(int i=0;i<atoi(argv[1])-1;i++) {
    pthread_cond_init(&(varCommunes.cond[i]),NULL);
  }

  
  srand(atoi(argv[1]));  // initialisation de rand pour la simulation de longs calculs
 
  // creation des threards 
  for (int i = 0; i < atoi(argv[1]); i++){
    // Initalisation de tabParams[i]
    tabParams[i].idThread = i;
    tabParams[i].varCommunes = &varCommunes; 
    // Lancement du tread
    if (pthread_create(&threads[i], NULL, traitement, &tabParams[i]) != 0){
      perror("erreur creation thread");
      exit(1);
    }
  }

  
  // attente de la fin des  threards. Partie obligatoire 
  for (int i = 0; i < atoi(argv[1]); i++){
    pthread_join(threads[i],NULL);
  }
  std::cout << "thread principal : fin de tous les threads secondaires" << std::endl;

  // liberer les ressources avant terminaison 
  for(int i=0;i<atoi(argv[1])-1;i++)
  {
    pthread_cond_destroy(&varCommunes.cond[i]);
  }
  pthread_mutex_destroy(&(varCommunes.verrou));
  free(varCommunes.cond);
  free(varCommunes.di);
  return 1;
}