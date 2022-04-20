#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>//perror
#include <unistd.h>
using namespace std;

// pour supprimer la file ayant la clé passée en parametre.

// une autre solution est d'utiliser la commande ipcrm -q <id_file>
// après avoir obtenue l'id de la file via la commande ipcs.

int main(int argc, char * argv[]){

  if (argc!=3) {
    cout<<"Nbre d'args invalide, utilisation :"<< endl;
    cout << argv[0] << " fichier-pour-cle-ipc entier-pour-cle-ipc"<< endl;
    exit(0);
  }
	  
  key_t cle=ftok(argv[1], atoi(argv[2]));

  if (cle==-1) {
    perror("Erreur ftok : ");
    exit(2);
  }

  cout << "ftok ok" << endl;
    
  int msgid = msgget(cle, 0666);
  if(msgid==-1) {
    perror("erreur msgget : ");
    exit(2);
  }
  
  cout << "msgget ok" << endl;
  struct demandeAcces
  {
    long etiq;
    int id;
  };

  struct sAutorisation
  {
    long etiq;
    char valeur;
  };

  struct finAccess
  {
    long etiq;
    int id;
  };
  int getid=getpid();
  while(1)
  {
    demandeAcces demande;
    demande.etiq=1;
    demande.id=getid;

    cout<<"Demande d'acces pret a etre envoye\n"<<endl;
    if(msgsnd(msgid,(void *)&demande,sizeof(int),0)==-1)
    {
      perror("Erreur lors de la demande d'envois\n");
      exit(1);
    }

    cout<<"Demande d'acces reussit\n"<<endl;

  sAutorisation autorisation;
  
  if(msgrcv(msgid,(void *)&autorisation,sizeof(char),long(getpid()),0)==-1)
  {
    perror("Erreur lors de la reception");
    exit(2);
  }

  cout<<"Autorisation d'acces\nLancement de mon processus"<<endl;

  sleep(2);
  cout<<"Processus fini\n"<<endl;


  finAccess fin;
  fin.etiq=2;
  fin.id=getid;

  if(msgsnd(msgid,(void *)&fin,sizeof(int),0)==-1)
  {
    perror("Erreur lors de l'envois\n");
    exit(2);
  }
  }
  


  
  return 0;
}
