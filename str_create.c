#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GenerateString(int n, char* string){
    if(n < 1)
        return;
    string[0] = 'a';
    char str;
    for(int i = 1; i < n; i++){
        str = (char)('a' + i);
        strcat(string, &str);
        strncpy(string + (1 << i), string, (1 << i) - 1);
    }
}

int main() {
    int n;
    scanf("%d", &n);
    char* a = calloc((1 << n), sizeof(char));

    GenerateString(n, a);

    printf("%s\n", a);

    free(a);
    return 0;
}
