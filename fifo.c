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

typedef struct PathFile{
    int maxLength;
    const char* path0;
    const char* path1;
} PathFile;

void strNull(char* a, int maxLength){
    for(int i = 0; i < maxLength; i++)
        a[i] = '\0';
}

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
}

void openMy(int* fd, const char* a){
    if ( (*fd = open(a , O_RDWR)) < 0) {
        printf("Can`t open FIFO.\n");
        exit(-1);
    } else
        printf("fd: %d\n", *fd);
}

int messanger(char** argv, PathFile* a){
    int fd[2], pid;
    char buf0[a->maxLength];
    char buf1[a->maxLength];
    strNull(buf0, a->maxLength);
    strNull(buf1, a->maxLength);

    makeFifo(a->path0);
    makeFifo(a->path1);

    if (argv[1] == NULL) {
        printf("Error. Enter the name of program in the following format: ./a.out 0(1)");
        return 0;
    } else if (argv[1][0] == '0') {
        printf("I'm the terminal 0.\n");
        printf("To exit enter \"qqq\"");
        openMy(fd, a->path0);
        openMy(fd+1, a->path1);

        pid = fork();
        if (pid == 0) {
            printf("Parent's pid: %d\nChild's pid: %d\n", getppid(), getpid());
        }
        if (pid > 0) {
            while (1) {
                strNull(buf1, a->maxLength);
                read(fd[1], buf1, a->maxLength);
                printf("Friend: %s\n", buf1);
            }
        } else if (pid == 0) {
            while (1) {
                strNull(buf0, a->maxLength);
                fgets(buf0, a->maxLength, stdin);
                if((buf0[0] == 'q') && (buf0[1] == 'q') && (buf0[2] == 'q')){
                    close(fd[0]);
                    close(fd[1]);
                    kill(getppid(), SIGTERM);
                    exit(0);
                }
                write(fd[0], buf0, strlen(buf0) - 1);
            }
        } else {
            printf("Cannot create a new process\n");
            exit(-1);
        }
    } else if (argv[1][0] == '1') {
        printf("I'm the terminal 1.\n");
        printf("To exit enter \"qqq\"");
        openMy(fd, a->path0);
        openMy(fd+1, a->path1);

        pid = fork();
        if (pid == 0) {
            printf("Parent's pid: %d\nChild's pid: %d\n", getppid(), getpid());
        }
        if (pid > 0) {
            while (1) {
                strNull(buf0, a->maxLength);
                read(fd[0], buf0, a->maxLength);
                printf("Friend: %s\n", buf0);
            }
        } else if (pid == 0) {
            while (1) {
                strNull(buf1, a->maxLength);
                fgets(buf1, a->maxLength, stdin);
                if((buf1[0] == 'q') && (buf1[1] == 'q') && (buf1[2] == 'q')){
                    close(fd[0]);
                    close(fd[1]);
                    kill(getppid(), SIGTERM);
                    exit(0);
                }
                write(fd[1], buf1, strlen(buf1) - 1);
            }
        } else {
            printf("Cannot create a new process\n");
            exit(-1);
        }
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

    messanger(argv, &a);

    return 0;
}
