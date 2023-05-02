#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#define TAILLE_ENREG 10
#define N 50

#define SEM_ERR 1
#define B1_ERR 2
#define B2_ERR 3

typedef struct enregistrement{
    char chaine[TAILLE_ENREG];
}enregistrement;


int semaphores;
int memb1,memb2;
int main(void){
    key_t clef ;
    int processus;
    int nombre_semaphore=4;
    /*creation des semaphores*/
    clef=ftok("main.c",1);
    semaphores=semget(clef,nombre_semaphore,IPC_CREAT|0666);
    if(semaphores==-1){printf("erreur creation des semaphores main\n");exit(SEM_ERR);}
    /*initialisation des semaphores*/
    semctl(semaphores,0,SETVAL,0);//plein1
    semctl(semaphores,1,SETVAL,0);//plein2
    semctl(semaphores,2,SETVAL,N);//vide1
    semctl(semaphores,3,SETVAL,N);//vide2
    /*creation de la memoire partagé B1*/
    clef=ftok("main.c",2);
    memb1=shmget(clef,N*sizeof(enregistrement),IPC_CREAT|0666);
    if(memb1==-1){printf("erreur de la creation de la memoire b1");exit(B1_ERR);}
    /*creation de la memoire partagé de B2*/
    clef=ftok("main.c",3);
    memb2=shmget(clef,N*sizeof(enregistrement),IPC_CREAT|0666);
    if(memb2==-1){printf("erreur de creation de la memoire B2");exit(B2_ERR);}
    /*creation des processus fils*/
    /*creation du p1*/
    processus=fork();
    if(processus==0){
        execl("./p1","p1",NULL);
    }
    /*creation du p2*/
    processus=fork();
    if(processus==0){
        execl("./p2","p2",NULL);
    }
    /*creation du p3*/
    processus=fork();
    if(processus==0){
        execl("./p3","p3",NULL);
    }
    while(wait(NULL)!=-1){
        printf("\nfils terminé\n");
    }
    /*destruction des semaphores et les memoires partagé*/
    semctl(semaphores,IPC_RMID,0);  
    shmctl(memb1,IPC_RMID,0);
    shmctl(memb2,IPC_RMID,0);
}