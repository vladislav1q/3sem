#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Split(char* str, char* delimeters, char** words, int* count){
    int res = 0;
    for(int i = 0, a = 0; i < 3000, *count < 100, a < 30; i++) {
        if (str[i] == '\0')
            break;
        for(int t = 0; t < 10; t++){
            if((int)str[i] == (int)delimeters[t]){
                res++;
                break;
            }
        }
        if(res == 0){
            words[*count][a] = str[i];
            a++;
        } else if(a > 0){
            (*count)++;
            a = 0;
        }
        res = 0;
    }
}

int main() {
            //GIVE ALL MEMORY
    char delimeters[10] = {' ', ',', '.', ';', ':', '(', ')', '!', '\'', '\"',};
    char* str = calloc(4000, sizeof(char));
    char** words = calloc(100, sizeof(char*));
    for(int i = 0; i < 100; i++){
        words[i] = calloc(30, sizeof(char));
    }
    int count = 0;

    FILE* f;
    if((f = fopen("/home/vladislav/CLionProjects/split/check.txt", "a+")) == NULL)
        printf("There is no this file!\n");
    fgets(str, 4000, f);

    Split(str, delimeters, words, &count);
    printf("Count of words: %d\n", count);
    for(int i = 0; i < count; i++){
        printf("\"%s\" ", words[i]);
    }
    printf("\n");

            //FREE ALL MEMORY
    for(int i = 0; i < 100; i++){
        free(words[i]);
    }
    free(words);

    return 0;
}