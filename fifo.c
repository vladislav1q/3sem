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

// FIXIT: константы, объявленные через define пишите, пожалуйста, вот в таком стиле MAX_LENGTH
#define maxLength 500

typedef struct PathFile{
    // FIXIT: для path0 и path1 вероятно можно придумать более "говорящие" названия: fileToWrite, fileToRead или что-то вроде того 
    const char* path0;
    const char* path1;
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
    // FIXIT: зачем вам два буффера? все равно только одна ветка if-а работать будет при заданном pid
    char buf0[maxLength];
    char buf1[maxLength];
    memset(buf1, 0, maxLength);
    memset(buf0, 0, maxLength);

    if (pid > 0) {
        while (1) {
            memset(buf1, 0, maxLength);
            read(fd[1], buf1, maxLength);
            printf("Friend: %s\n", buf1);
        }
    } else if (pid == 0) {
        printf("Parent's pid: %d\nChild's pid: %d\n\n", getppid(), getpid());
        
        while (1) {
            memset(buf0, 0, maxLength);
            fgets(buf0, maxLength, stdin);
            // FIXIT: условие ниже лучше через strcmp реализовать
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
}

int createMessanger(char** argv, PathFile* a){
    int fd[2], pid;

    makeFifo(a->path0);
    makeFifo(a->path1);

    if (argv[1] == NULL) {
        printf("Error. Enter the name of program in the following format: ./a.out 0(1)\n");
        return 1;
    } else if (argv[1][0] == '0') {
        // FIXIT: код в разных ветка if-а отличается минимально. Чтобы избежать дублирования также давайте в отдельную ф-ю вынесем
        printf("I'm the terminal 0.\nTo exit enter \"qqq\"\n");

        openMy(fd+1, a->path1);
        openMy(fd, a->path0);
        
        pid = fork();
        createUser(fd, pid);

    } else if (argv[1][0] == '1') {
        printf("I'm the terminal 1.\nTo exit enter \"qqq\"\n");

        openMy(fd+1, a->path0);
        openMy(fd, a->path1);

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
    a.path0 = "a1.fifo";
    a.path1 = "a2.fifo";

    status = createMessanger(argv, &a);

    (status != 0) ? printf("Exit with error %d.\n", status) : printf("Exit without mistakes.\n");

    return 0;
}
