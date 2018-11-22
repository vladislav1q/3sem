#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void toDo(int semid, int number, int action){
    struct sembuf mybuf;

    mybuf.sem_num = number;
    mybuf.sem_op  = action;
    mybuf.sem_flg = 0;

    semop(semid, &mybuf, 1);
}

int main(){
    struct mybuf{
        long mtype;
        int x, y, pid;
    } mybuf;

    int msqid, semid, length;
    char pathname[] = "server.c";
    key_t key;

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((semid = semget(key, 1, 0)) > 0)
        semctl(semid, IPC_RMID, 0);

    semid = semget(key, 1, 0666 | IPC_CREAT);
    toDo(semid, 0, 2);

    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-2);
    }

    printf("The work was started\n\n");
    while(1){
        if(( length = msgrcv(msqid, &mybuf, sizeof(mybuf) - sizeof(long), 1, 0) < 0)){
            printf("Can\'t receive message from queue\n");
            exit(-3);
        }

        toDo(semid, 0, -1);
        int pid = fork();
        if(pid == 0)
        {
            int result;
            mybuf.x *= mybuf.y;
            mybuf.mtype = mybuf.pid;

            if (msgsnd(msqid, &mybuf, sizeof(mybuf) - sizeof(long), 0) < 0){
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, NULL);
                exit(-4);
            }
            toDo(semid, 0, 1);
            printf("Calculations were ended for pid: %d\n\n", mybuf.pid);
        }
    }

    return 0;
}
