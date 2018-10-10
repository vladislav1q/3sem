#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

// Вы переименовывали названия структур a и b на что-то более понятное, но в сюда изменения не перенесли

typedef struct a{
    int str, word;
}Max;

typedef struct c{
    unsigned int num, pid;
}Pid;

typedef struct b{
    int count;
    char **str;
    char *delimeters;
    char ***words;
    Max *max;
} All;

void split(All* a, int i){
    a->words[i][a->count] = strtok(a->str[i], a->delimeters);

    while(a->words[i][a->count] != NULL){
        a->count++;
        a->words[i][a->count] = strtok(NULL, a->delimeters);
    }
}

void giveMemory(All* a, int count){
    a->max = calloc(1, sizeof(Max));
    a->max->str = 4000;
    a->max->word = 40;
    a->count = 0;
    a->str = calloc(count, sizeof(char*));
    a->words = calloc(count, sizeof(char**));
    for(int i = 0; i < count; i++) {
        a->str[i] = calloc(a->max->str, sizeof(char));
        a->words[i] = calloc(a->max->word, sizeof(char *));
    }
}

void freeMemory(All* mem, int count){
    for(int i = 0; i < count; i++) {
        free(mem->words[i]);
        free(mem->str[i]);
    }
    free(mem->words);
    free(mem->str);
    free(mem->max);
}

int compare(const void *x, const void *y){
    const Pid *a = (const Pid*)x;
    const Pid *b = (const Pid*)y;
    return a->num - b->num;
}

int main(){
    int count = 0, delay;
    scanf("%d\n", &delay);
    char delimeters[] = " \n";
    int maxNumberStr = 100;

    All a;
    giveMemory(&a, maxNumberStr);
    a.delimeters = delimeters;

    while(fgets(a.str[count], a.max->str, stdin)){  //getting strings and the number of them
        printf("%s", a.str[count]);
        a.count = 0;
        split(&a, count);
        count++;
    }
    count--;

    Pid child[count];                               //create massiv with delays and pids of processes
    int pid;
    for(int i = 0; i < count; i++){
        child[i].num = (unsigned int)atoi(a.words[i][0]);
    }

    for(int i = 0; i < count; i++){                 //create a new process with determinied delay
        pid = fork();
        child[i].pid = pid;
        if(pid == 0){
            sleep(child[i].num);
            execvp(a.words[i][1], &(a.words[i][1]));
            exit(0);
        } else if(pid == -1){
            printf("Error!\n");
            return -1;
        }
    }

    // FIXIT: вы сортируете по pid, а не по времени старта. Зато, как я понял, ниже предполагаете, что у в child[count - 1].num максимальное значение
    qsort(child, count, sizeof(Pid), compare);

    sleep(delay);
    for(int i = 0; i <= child[count-1].num; i++){   //killing processes
        for(int k = 0; k < count; k++)
            if(child[k].num == i){
                // FIXIT: можно удостовериться с помощью waitpid с флаго NOHANG, что процесс все ещё живой и только тогда убивать и писать сообщение об этом
                kill(child[k].pid, SIGKILL);
                printf("Process with pid %d is killed\n", child[k].pid);
            }
        // Здорово, что вы придумали альтернативный способ учета таймаута, предполагая, что все времена кратны 1 сек
        sleep(1);
    }

    freeMemory(&a, maxNumberStr);

    return 0;
}
