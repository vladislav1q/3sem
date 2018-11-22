#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    struct mybuf{
        long mtype;
        int x, y, pid;
    } mybuf;
    mybuf.mtype = 1;
    mybuf.pid = getpid();

    int msqid, x, y, i, length;
    char pathname[] = "server.c";
    key_t key;

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-2);
    }

    scanf("%d%d", &(mybuf.x), &(mybuf.y));
    printf("my pid: %d\n", mybuf.pid);
    if (msgsnd(msqid, &mybuf, sizeof(mybuf)-sizeof(long), 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, NULL);
        exit(-3);
    }

    if(( length = msgrcv(msqid, &mybuf, sizeof(mybuf)- sizeof(long), getpid(), 0) < 0)){
        printf("Can\'t receive message from queue\n");
        exit(-4);
    }
    printf("multiplication: %d\n", mybuf.x);

    return 0;
}
