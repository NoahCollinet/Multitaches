
/*
 Programme ronde avec un thread bus et des threads visiteurs. 
 Les zones � compl�ter sont indiqu�es en commentaires.
 
 Les traces fournies sont suffisantes.
 
 Vous avez � votre disposition test/zoo qui est un ex�cutable vous illustrant le comportement attendu.
 
*/

#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"

struct varPartagees {
  // structure regroupant toutes le varables partag�es entre threads
  int * nbPlaces; // nombre total de places dans le bus
  pthread_mutex_t * verrou_place; //verrou pour une place
  pthread_cond_t * condi;//condition
  int visite;
};


struct params {

  // structure pour regrouper les param�tres d'un thread. 
  int idThread; // un identifiant de visiteur, de 1 � N (N le nombre total de visiteurs)
  struct varPartagees * varP;
};

// pour le thread bus
void * bus (void * p) {

  struct params * args = (struct params *) p;
  struct varPartagees * varP  = args -> varP;
  
  while(1){
  
    afficher('b', "embarquement immediat, soyez les bienvenus!", 0);
    
    // ... zone a compl�ter pour attendre que le bus soit plein
      
    pthread_mutex_lock(args->varP->verrou_place);

    afficher('b', "attente que le bus soit complet", 0);

    printf("J'ai %i places libres (before)\n",*(args->varP->nbPlaces));

    monterOuDescendre();
    (*(args->varP->nbPlaces))--;

    printf("J'ai %i places libres (after)\n",*(args->varP->nbPlaces));
    
      if(*(args->varP->nbPlaces)!=0)
      {
        pthread_cond_wait(args->varP->condi, args->varP->verrou_place);
      }
      else
      {
          pthread_cond_broadcast(args->varP->condi);
      }

    pthread_mutex_unlock(args->varP->verrou_place);
  
    afficher('b',"bus complet, la visite commence !", 0); 
    (args->varP->visite) = 1 ;
    visite(5); // vous pouvez changer la valeur du param�tre (voir .h)
    (args->varP->visite) = 0 ;
    afficher('b',"visite terminee, tout le monde descend !", 0);
      
    pthread_mutex_lock(args->varP->verrou_place);

    afficher('b', "attente que tout le monde descende", 0);

    printf("J'ai %i places libres \n",*(args->varP->nbPlaces));

    monterOuDescendre();
    (*(args->varP->nbPlaces))++;

    if (*(args->varP->nbPlaces)!=argv[0]))
    {
      pthread_cond_wait(args->varPartagees->condi, args->varP);
    }
    
    else
    {
      pthread_cond_broadcast(args->varP->condi);
    }
    
    pthread_mutex_unlock(args->varP->verrou_place);

    pthread_exit(NULL); 
  }
}


// pour le thread visiteur
void * visiteur (void * p) {

  struct params * args = (struct params *) p;
  struct  varPartagees * varP = args -> varP;
  
   afficher('v', "je demande a monter", args -> idThread);
   
   //... zone a compl�ter pour mettre en place la mont�e.
   pthread_mutex_lock(args->varP->verrou_place);
   if (*(args->varP->nbPlaces)=0)
   {
     afficher('v', "pas de place, j'attends", args -> idThread);
   }
   pthread_mutex_unlock(args->varP->verrou_place);
   // simulation mont�e du visiteur
   afficher('v', "je monte ...", args -> idThread);
  
   monterOuDescendre();
   
   afficher('v', "je suis a bord et bien installe !", args -> idThread);
   
  // ... zone qui peut, en fonction de votre solution, �tre utile pour compl�ter la mise en place de la mont�e.
   
   // ici se produit automatiquement la visite qui est g�r�e par le bus
  if ((args->varP->visite) = 1)
  {
    afficher('v', "j'attends la fin de la visite", args -> idThread);
  }
  
  
   afficher('v', "visite terminee, je descends ...", args -> idThread);
   // .. zone qui pourrait �ventuellement vous �tre utile
   monterOuDescendre();
   // .. zone qui pourrait �ventuellement vous �tre utile
 
   afficher('v', "je suis descendu !", args -> idThread);
   
  // ... zone qui peut, en fonction de votre solution, �tre utile pour compl�ter la mise en place de la descente.
   
  pthread_exit(NULL); 
}


int main(int argc, char * argv[]){
  
  if (argc!=3) {
    printf(" argument requis \n %s nombres_places_bus nombre_visiteurs\n", argv[0]);
    exit(1);
  }

 initDefault(atoi(argv[2])); // ne pas modifier cet appel ni le d�placer.
 
 
  // initialisations 
  pthread_t threads[atoi(argv[2])+1];
  struct params tabParams[atoi(argv[2])+1];
 
  struct varPartagees * varP;
  
  varP->nbPlaces = * atoi(argv[1]) ;
  
  pthread_mutex_t verrou_place;
  pthread_mutex_init(&verrou_place, NULL);
  varP->verrou_place = &verrou_place;

  pthread_cond_t condi;
  pthread_cond_init(&condi,NULL);
  varP->condi=&condi;

 
  // cr�ation des threads
  tabParams[0].idThread = 0;
  tabParams[0].varPartagees = &varP; 
  if (pthread_create(&(threads[0]), NULL, bus, &(tabParams[0])) != 0){
      perror("erreur creation thread bus");
      exit(1);
    }  
  for (int i = 1; i < atoi(argv[2])+1; i++){
    tabParams[i].idThread = i;
    tabParams[i].varPartagees = &varP; 
    if (pthread_create(&threads[i], NULL, visiteur, &(tabParams[i])) != 0){
      perror("erreur creation thread visiteur");
      exit(1);
    }
  }
  
printf("creation de thread reussie\n");

  // attente de la fin des  threads. 
  for (int i = 0; i < atoi(argv[2])+1; i++)
    if (pthread_join(threads[i], NULL) != 0){
      perror("erreur attente thread");
      exit(1);
    }
    
  // ... zone � compl�ter pour terminer proprement votre programme.
 
  return 0;
}
 
