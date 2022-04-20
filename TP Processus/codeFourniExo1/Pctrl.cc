#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>//perror
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
    
  int msgid = msgget(cle,IPC_CREAT|IPC_EXCL|0666);
  if(msgid==-1) {
    perror("erreur msgget : ");
    exit(2);
  }
  
  cout << "msgget ok" << endl;
    struct demandeAcces
      {
          long etiq; //1: demande d'acces
          int idDemandeur;
      };

      struct sautorisation
      {
          long etiq; //id demandeur
          char valeur; //Pour envoyer un truc comme ca
      };

    struct finAccess
    {
        long etiq; //2: fin acces
        int idDemandeur;
    };
  while(1)
  {
    demandeAcces demande;
    cout<<"Attente demande d'acces"<<endl;
    if(msgrcv(msgid,(void *)&demande, sizeof(int), long(1),0)==-1)
    {
        perror("Reception demande autorisation");
        exit(2);
    }
    cout<<"Demande d'access recue\nEtiquette: "<<demande.etiq<< "\nEt l'id demande est: "<<demande.idDemandeur<<endl;


    sautorisation autorisation;

    autorisation.etiq=demande.idDemandeur;
    autorisation.valeur='a';


    if(msgsnd(msgid, (void *)&autorisation,sizeof(char),0)==-1)
    {
        perror("Erreur envoi autorisation:");
        exit(2);
    }

    finAccess fin;

    cout<<"Attente liberation de la ressource"<<endl;
    if(msgrcv(msgid, (void *)&fin,sizeof(int),long(2),0)==-1)
    {
        perror("Reception notification\nFin d'acces");
        exit(2);
    }

    cout<<"Contenu du message lu:\nEtiquette: "<<fin.etiq<<"\nEt l'id de demande est: "<<fin.idDemandeur<<endl; 


    }
  
  return 0;
}
