#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main() {

  int shmid = shmget(ftok("client.c", 23), ___, IPC_CREAT | IPC_EXCL | 0644);
  char *shm = shmat(shmid, 0, 0);

  int semid = semget(ftok("makefile", 22), 1, 0);
  printf("[%d] before access\n", getpid());

  srand(time(NULL));

  int x = (rand() % 10) + 5;

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;

  semop(semid, &sb, 1);
  printf("[%d] I’m in!\n", getpid());

  sleep(x);

  sb.sem_op = 1;
  semop(semid, &sb, 1);

  printf("[%d] I’m done!\n", getpid());

  return 0;
}
