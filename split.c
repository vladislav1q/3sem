#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct a{
    int count;
    char* str;
    char* delimeters;
    char** words;
} All;

void Split(All* a){
    int res = 0;
    for(int i = 0, c = 0; i < 3000, a->count < 100, c < 30; i++) {
        if (a->str[i] == '\0')
            break;
        for(int t = 0; t < 10; t++){
            if((int)a->str[i] == (int)a->delimeters[t]){
                res++;
                break;
            }
        }
        if(res == 0){
            a->words[a->count][c] = a->str[i];
            c++;
        } else if(a > 0){
            (a->count)++;
            c = 0;
        }
        res = 0;
    }
}

void GiveMemory(All* a){
    a->delimeters = calloc(10, sizeof(char));
    a->str = calloc(4000, sizeof(char));
    a->words = calloc(100, sizeof(char*));
    for(int i = 0; i < 100; i++){
        a->words[i] = calloc(30, sizeof(char));
    }
    a->count = 0;
}

void Scan(All* a){
    FILE* f, *ff;
    char c[10] = {NULL};
    int i = 0;
    if((f = fopen("/home/vladislav/CLionProjects/split/check.txt", "a+")) == NULL)
        printf("There is no this file!\n");
    if((ff = fopen("/home/vladislav/CLionProjects/split/delimeters.txt", "a+")) == NULL)
        printf("There is no this file!\n");
    fgets(a->str, 4000, f);
    fgets(c, 10, ff);
    while(c[i] != NULL && i < 10){
        a->delimeters[i] = c[i];
        i++;
    }
}

void FreeMemory(All* mem){
    for(int i = 0; i < 100; i++){
        free(mem->words[i]);
    }
    free(mem->words);
    free(mem->delimeters);
    free(mem->str);
}

int main() {
    All a;
    GiveMemory(&a);
    Scan(&a);

    Split(&a);
    printf("Count of words: %d\n", a.count);
    for(int i = 0; i < a.count; i++){
        printf("\"%s\" ", a.words[i]);
    }
    printf("\n");
    FreeMemory(&a);

    return 0;
}
