#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

static const uint16_t SRV_PORT = 13666;

void readIp(char *ip) {
    bzero(ip, 16);
    FILE* f = popen("hostname -I", "r");
    fread(ip, sizeof(char), 16, f);
    fclose(f);
}

void readName(char *myName){
    bzero(myName, sizeof(&myName));
    printf("Enter your name: \n");
    fgets(myName, 49, stdin);
}

void fill(const char *ip, struct sockaddr_in *servAddr) {
    bzero(servAddr, sizeof(*servAddr));
    (*servAddr).sin_family = AF_INET;
    (*servAddr).sin_addr.s_addr = inet_addr(ip);
    (*servAddr).sin_port = htons(SRV_PORT);
}

void* readServ(void *arg){
    int sd = *(int *) arg;
    char buf[512];
    ssize_t test;
    while (1) {
        bzero(buf, sizeof(buf));
        test = read(sd, buf, sizeof(buf) - 1);
        if (test < 0)
            exit(5);
        printf("%s", buf);
    }
}

int main() {
    char ip[16];
    char name[52];
    readIp(ip);
    readName(name);
    strcpy(&name[strlen(name)-1], ": ");

    struct sockaddr_in servAddr;
    fill(ip, &servAddr);

    char buf[512];
    int fdWrite = socket(AF_INET, SOCK_STREAM, 0);
    if (fdWrite < 0) {
        printf("socket write failed\n");
        exit(1);
    }

    int fdRead = socket(AF_INET, SOCK_STREAM, 0);
    if (fdRead < 0) {
        printf("socket read failed\n");
        exit(2);
    }

    if (connect(fdWrite, (const struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        printf("connect failed\n");
        close(fdWrite);
        exit(3);
    }

    if (connect(fdRead, (const struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("connect failed\n");
        close(fdWrite);
        exit(4);
    }

    pthread_t thid;
    int result = pthread_create(&thid, (pthread_attr_t *) NULL, readServ, &fdRead);
    if (result != 0) {
        printf("Error on thread create, return value = %d\n", result);
        exit(5);
    }

    while(1){
        bzero(buf, sizeof(buf));
        strcpy(buf, name);
        fgets(&buf[strlen(buf)], 450, stdin);

        if (write(fdWrite, buf, (strlen(buf)+1)*sizeof(char)) < 0) {
            printf("write failed\n");
            close(fdWrite);
            exit(6);
        }
    }
    close(fdWrite);
    return 0;
}
