#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main() {

  //Setup shared memory	
  int shmkey = ftok("client.c", 23);
  int shmid = shmget(shmkey, sizeof(int), 0644);
  char *shm = shmat(shmid, 0, 0);
  //*shm stores sizeof previous submission

  //Setup semaphore
  int semkey = ftok("makefile", 22);
  int semid = semget(semid, 1, 0);

  //Attempt to down the semaphore
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;

  //Down
  semop(semid, &sb, 1);
  
  //Move to last submission position in file for reading
  int fd = open("story.txt", O_RDWR | O_APPEND, 0644);
  lseek(fd, -(*shm), SEEK_END);
  //buf is string of last submission
  char *buf = (char *) malloc(*shm + 1); 
  read(fd, buf, *shm); 
  printf("Previously in this story . . . : %s\n", buf);

  //Take input and write
  char store[2048];
  fgets(store, 2048, stdin);
  write(fd, store, strlen(store));
  close(fd);
  
  //Store new input size into shared memory
  *shm = strlen(store);
  shmdt(shm);

  //Up the semaphore when done
  sb.sem_op = 1;
  semop(semid, &sb, 1);
  printf("Your submission has been submitted\n");

  return 0;
}
