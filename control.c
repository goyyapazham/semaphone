#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

/*
union semun{
  int val;
  struct seid_ds *bus;
  unsigned short *array;
  struct seminfo *__buf;
}
 */

int main(int argc, char *argv[]){
  int semid;
  int key = ftok("makefile" , 22);
  int sc;
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
    printf("semaphore createdL %d\n", semid);
    union semun su;
    su.val = atoi(argv[2]);
    //setting semaphore value
    sc = semctl(semid, 0, SETVAL, su);
    printf("value set: %d\n", sc);
  }
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    semid = semget(key, 1, 0);
    //getting the value of a semaphore
    sc = semctl(semid, 0, GETVAL);

    printf("semaphore value: %d\n",sc);
  }
  else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
    semid = semget(key, 1, 0);
    //removing a semaphore
    sc = semctl(semid, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sc);
  }
  return 0;

}
