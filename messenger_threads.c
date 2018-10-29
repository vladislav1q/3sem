#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <asm/errno.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

typedef struct PathFile{
    int maxLength;
    const char* path0;
    const char* path1;
} PathFile;

typedef struct Messenger{
    char* buf0, *buf1;
    int* fd;
    int maxLength;
} Messenger;

void strNull(char* a, int maxLength){
    for(int i = 0; i < maxLength; i++)
        a[i] = '\0';
};

void makeFifo(const char* path){
    umask(0);
    if(mkfifo(path, 0666) < 0){
        if (errno == EEXIST)
            printf("Fifo file %s exists.\n", path);
        else{
            printf("Can`t create FIFO\n");
            exit(-1);
        }
    } else
        printf("The file %s was created\n", path);
};

void openMy(int* fd, const char* a){
    if ( (*fd = open(a , O_RDWR)) < 0) {
        printf("Can`t open FIFO.\n");
        exit(-1);
    } else
        printf("fd: %d\n", *fd);
};

void* myRead(void* info){
    Messenger* myInfo = (Messenger *)info;
    while(1){
        strNull(myInfo->buf0, myInfo->maxLength);
        read(myInfo->fd[0], myInfo->buf0, myInfo->maxLength);
        printf("Friend: %s\n", myInfo->buf0);
    }
};

void* myWrite(void* info){
    Messenger* myInfo = (Messenger *)info;
    while(1){
        strNull(myInfo->buf1, myInfo->maxLength);
        fgets(myInfo->buf1, myInfo->maxLength, stdin);
        if((myInfo->buf1[0] == 'q') && (myInfo->buf1[1] == 'q') && (myInfo->buf1[2] == 'q')){
            close(myInfo->fd[0]);
            close(myInfo->fd[1]);
            system("rm a1.fifo");
            system("rm a2.fifo");
            exit(0);
        }
        write(myInfo->fd[1], myInfo->buf1, strlen(myInfo->buf1) - 1);
    }
};

int messenger(char** argv, PathFile* a) {
    int fd[2];
    char buf0[a->maxLength];
    char buf1[a->maxLength];
    strNull(buf0, a->maxLength);
    strNull(buf1, a->maxLength);

    Messenger info;
    info.buf0 = buf0;
    info.buf1 = buf1;
    info.fd = fd;
    info.maxLength = a->maxLength;

    int result1, result2;
    pthread_t thid, thid2;

    makeFifo(a->path0);
    makeFifo(a->path1);

    if (argv[1] == NULL) {
        printf("Error. Enter the name of program in the following format: ./a.out 0(1)");
        return 0;
    } else if (argv[1][0] == '0') {
        printf("I'm the terminal 0.\n");
        printf("To exit enter \"qqq\"\n");

        openMy(fd, a->path0);
        openMy(fd + 1, a->path1);

        result1 = pthread_create(&thid, (pthread_attr_t *) NULL, myRead, &info);
        result2 = pthread_create(&thid2, (pthread_attr_t *) NULL, myWrite, &info);

        if (result1 != 0) {
            printf("Error on thread create, return value = %d\n", result1);
            exit(-1);
        }
        if (result2 != 0) {
            printf("Error on thread create, return value = %d\n", result2);
            exit(-1);
        }
        pthread_join(thid, (void **) NULL);
        pthread_join(thid2, (void **) NULL);
    } else if (argv[1][0] == '1') {
        printf("I'm the terminal 1.\n");
        printf("To exit enter \"qqq\"\n");
        openMy(fd + 1, a->path0);
        openMy(fd, a->path1);

        result1 = pthread_create(&thid, (pthread_attr_t *) NULL, myRead, &info);
        result2 = pthread_create(&thid2, (pthread_attr_t *) NULL, myWrite, &info);

        if (result1 != 0) {
            printf("Error on thread create, return value = %d\n", result1);
            exit(-1);
        }
        if (result2 != 0) {
            printf("Error on thread create, return value = %d\n", result2);
            exit(-1);
        }
        pthread_join(thid, (void **) NULL);
        pthread_join(thid2, (void **) NULL);
    } else {
        printf("Error. Enter the name of program in the following format: ./a.out 0(1)");
        return 0;
    }
}

int main(int argc, char *argv[], char *envp[]) {
    PathFile a;
    a.maxLength = 500;
    a.path0 = "a1.fifo";
    a.path1 = "a2.fifo";

    makeFifo(a.path0);
    makeFifo(a.path1);

    messenger(argv, &a);

    return 0;
}
