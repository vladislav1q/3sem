#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct a {
    int str, delimeters, word;
} Max;

typedef struct b {
    int count;
    char* str;
    char* delimeters;
    char** words;
    Max *max;
} All;

// Все хорошо, кроме названия структур Max и All. Уж больно непонятные названия.Еще и "a", "b" :)
// Обычно пишут typedef struct All { ... } All; без дополнительного имени структуры.

void split(All* a) {
    a->words[a->count] = strtok(a->str, a->delimeters);

    while(a->words[a->count] != NULL){
        a->count++;
        a->words[a->count] = strtok(NULL, a->delimeters);
    }
}

void giveMemory(All* a) {
    a->max = calloc(1, sizeof(Max));
    a->max->str = 4000;
    a->max->delimeters = 50;
    a->max->word = 40;
    a->delimeters = calloc(a->max->delimeters, sizeof(char));
    a->str = calloc(a->max->str, sizeof(char));
    a->words = calloc(a->max->word, sizeof(char*));
    a->count = 0;
}

void freeMemory(All* mem){
    free(mem->words);
    free(mem->delimeters);
    free(mem->str);
}

void scan(All* a) {
    printf("Write a string, you want to divide\n");
    fgets(a->str, a->max->str, stdin);
    printf("Write a string of delimeters\n");
    fgets(a->delimeters, a->max->delimeters, stdin);
}

int main() {
    All a;
    giveMemory(&a);
    scan(&a);
    split(&a);

    printf("Count of words: %d\n", a.count);
    for(int i = 0; i < a.count; i++){
        printf("\"%s\" ", a.words[i]);
    }
    printf("\n");

    freeMemory(&a);
    return 0;
}
