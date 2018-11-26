#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

int depth = 2;

void find(char *str, char* cmp, int i){
    if(i > depth)
        return;
    DIR* a;
    a = opendir(str);

    if(a == NULL){
        if(errno == EACCES){
            printf("Permission denied %s\n", str);
        } else if(errno == ENOTDIR){
            printf("Is not a directory %s\n", str);
        } else if(errno == EBADF){
            printf("fd is not a valid file descriptor opened for reading %s\n", str);
        } else if(errno  == EMFILE){
            printf("The per-process limit on the number of open file descriptors has been reached %s\n", str);
        } else if(errno == ENFILE){
            printf("The system-wide limit on the total number of open files has been reached %s\n", str);
        } else if(errno == ENOENT){
            printf("Directory does not exist, or name is an empty string(more common it is link to other directory %s\n", str);
        } else if(errno == ENOMEM){
            printf("Insufficient memory to complete the operation %s\n", str);
        } else{
            printf("Can't read\n");
        }

        return;
    }

    char* str2 = malloc(400*sizeof(char));
    char* cmp2 = malloc(400*sizeof(char));

    struct dirent* b;
    struct stat buf;

    while((b = readdir(a)) != NULL){
        strcpy(str2, str);
        strcat(str2, "/");
        strcat(str2, b->d_name);
        //printf("in directory: %s\n", str2);
        stat(str2, &buf);

        if(S_ISDIR(buf.st_mode)){
            if((strcmp(b->d_name, ".") == 0) || (strcmp(b->d_name, "..") == 0))
                continue;
            //printf("Started finding in %s\n\n", str2);
            find(str2, cmp, i+1);
        } else if(S_ISLNK(buf.st_mode)) {
            continue;
        } else {
            strcpy(cmp2, str);
            strcat(cmp2, "/");
            strcat(cmp2, cmp);
            if(strcmp(cmp2, str2) == 0)
                printf("FOUND: %s\n", cmp2);
        }
        memset(str2, (int)'\0', 400);
        memset(cmp2, (int)'\0', 400);
    }
    free(str2);
    free(cmp2);
    closedir(a);
}

int main() {
    char* str = malloc(100* sizeof(char));
    char* cmp = malloc(20* sizeof(char));

    printf("Write the depth of search\n");
    scanf("%d", &depth);

    printf("Where:\n");
    scanf("%s", str);

    printf("What:\n");
    scanf("%s", cmp);

    if(str[strlen(str)-1] == '/')
        str[strlen(str)-1] = '\0';
    find(str, cmp, 1);



    return 0;
}