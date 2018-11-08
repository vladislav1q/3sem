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

#define MAX_Length 500

typedef struct PathFile{
    const char* fileToWrite;
    const char* fileToRead;
} PathFile;

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

int createUser(int *fd, int pid){
    char buf0[MAX_Length];
    memset(buf0, 0, MAX_Length);

    if (pid > 0) {
        while (1) {
            memset(buf0, 0, MAX_Length);
            read(fd[1], buf0,MAX_Length);
            printf("Friend: %s\n", buf0);
        }
    } else if (pid == 0) {
        printf("Parent's pid: %d\nChild's pid: %d\n\n", getppid(), getpid());

        while (1) {
            memset(buf0, 0, MAX_Length);
            fgets(buf0, MAX_Length, stdin);
            if(strncmp(buf0, "qqq", 3) == 0){
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
}

int createMessanger(char** argv, PathFile* a){
    int fd[2], pid;

    makeFifo(a->fileToWrite);
    makeFifo(a->fileToRead);

    if (argv[1] == NULL) {
        printf("Error. Enter the name of program in the following format: ./a.out 0(1)\n");
        return 1;
    } else if ((argv[1][0] == '0') || (argv[1][0] == '1')) {
        int user = (int) argv[1][0] - (int) '0';
        printf("I'm the terminal %d.\nTo exit enter \"qqq\"\n", user);

        openMy(fd + user, a->fileToWrite);
        openMy(fd + (user + 1) % 2, a->fileToRead);

        pid = fork();
        createUser(fd, pid);
    } else {
        printf("Error. Enter the name of program in the following format: ./a.out 0(1)\n");
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[], char *envp[]) {
    int status;
    PathFile a;
    a.fileToWrite = "a1.fifo";
    a.fileToRead = "a2.fifo";

    status = createMessanger(argv, &a);

    (status != 0) ? printf("Exit with error %d.\n", status) : printf("Exit without mistakes.\n");

    return 0;
}
