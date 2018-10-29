#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

typedef struct MaxValue{
    int str, word;
} MaxValue;

typedef struct Pid{
    unsigned int time, pid;
} Pid;

typedef struct SplitParameters{
    int count;
    char **str;
    char *delimeters;
    char ***words;
    MaxValue *max;
} SplitParameters;

void split(SplitParameters* a, int i){
    a->words[i][a->count] = strtok(a->str[i], a->delimeters);

    while(a->words[i][a->count] != NULL){
        a->count++;
        a->words[i][a->count] = strtok(NULL, a->delimeters);
    }
}

void giveMemory(SplitParameters* a, int count){
    a->max = calloc(1, sizeof(MaxValue));
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

void freeMemory(SplitParameters* mem, int count){
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
    return a->time - b->time;
}

int main(){
    int count = 0, delay, status = 0;
    scanf("%d\n", &delay);
    char delimeters[] = " \n";
    int maxNumberStr = 100;

    SplitParameters a;
    giveMemory(&a, maxNumberStr);
    a.delimeters = delimeters;

    while(fgets(a.str[count], a.max->str, stdin)){  //getting strings and the number of them
        printf("%s", a.str[count]);
        a.count = 0;
        split(&a, count);
        count++;
    }

    Pid child[count];                               //create array with delays and pids of processes
    int pid;
    for(int i = 0; i < count; i++){
        child[i].time = (unsigned int)atoi(a.words[i][0]);
    }

    for(int i = 0; i < count; i++){                 //create a new process with determined delay
        pid = fork();
        child[i].pid = pid;
        if(pid == 0){
            sleep(child[i].time);
            execvp(a.words[i][1], &(a.words[i][1]));
            exit(0);
        } else if(pid == -1){
            printf("Error!\n");
            return -1;
        }
    }

    printf("\nCount is: %d\n", count);

    for(int i = 0; i < count; i++){                 //create a new process with determinied delay
        printf("My pid is %d\n", child[i].pid);
    }

    qsort(child, count, sizeof(Pid), compare);

    sleep(delay);
    for(int i = 0; i <= child[count - 1].time; i++){   //killing processes
        for(int k = 0; k < count; k++)
            if(child[k].time == i){
                if(!waitpid(child[k].pid, &status, CLD_EXITED)){
                    kill(child[k].pid, SIGKILL);
                    printf("Process with pid %d is killed\n", child[k].pid);
                }
            }
        sleep(1);
    }

    freeMemory(&a, maxNumberStr);

    return 0;
}
