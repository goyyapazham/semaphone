#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>

///*  Comment this line to uncmment the union declaration
union semun{
  int val;
  struct seid_ds *bus;
  unsigned short *array;
  struct seminfo *__buf;
};
//*/

void print(){
    //Print file
    int c;
    FILE *file = fopen("story.txt", "r");
    char store[2048];
    while (fgets(store, sizeof(store), file)){
    	printf("%s", store);
    }
    fclose(file);
}

int main(int argc, char *argv[]){
  int shmkey = ftok("client.c", 23);
  int semkey = ftok("makefile" , 22);
  int shmid;
  int semid;

  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    //Create semaphore
    semid = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0644);
    union semun su;
    su.val = 1;
    semctl(semid, 0, SETVAL, su);

    //Create shared memory
    shmid = shmget(shmkey, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);

    //Create file
    int fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
    close(fd);
  }

  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    print();
  }
  else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
    int sc;
    semid = semget(semkey, 1, 0);
    //removing a semaphore
    sc = semctl(semid, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sc);
    shmid = semget(shmkey, sizeof(int), 0);
    struct shmid_ds *buf;
    shmctl(shmid, IPC_RMID, buf);
    print();
  }
  return 0;

}
