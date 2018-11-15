#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int maxNumberInMatrix = 10;
int maxNumberLinesInMatrix = 10;

typedef struct Matrix{
    int height, width;
    double **matrix;
} Matrix;

typedef struct Thread{
    Matrix A, B, *C;
    int n;
} Thread;

void createMatrix(int height, int width, Matrix *A){
    A->width = width;
    A->height = height;

    A->matrix = malloc((A->height) * sizeof(double *));
    for(int i = 0; i < A->height; i++)
        (A->matrix)[i] = malloc((A->width) * sizeof(double));
};

void scanMatrix(Matrix *A){
    printf("Fill in the height and width of the matrix ( in the format: \"n m\" )\n");
    scanf("%d %d", &(A->height), &(A->width));

    createMatrix(A->height, A->width, A);

    printf("Write down the matrix.\n");
    for(int i = 0; i < A->height; i++){
        for(int k = 0; k < A->width; k++)
            scanf("%lf", &((A->matrix)[i][k]));
    }
}

void randMatrix(Matrix *A, int numberLines, int numberColumn){
    (numberColumn) ? (A->width = numberColumn) : (A->width = (int)random() % maxNumberLinesInMatrix);
    (numberLines) ? (A->height = numberLines) : (A->height = (int)random() % maxNumberLinesInMatrix);
    createMatrix(A->height, A->width, A);

    for(int i = 0; i < A->height; i++){
        for(int k = 0; k < A->width; k++)
            (A->matrix)[i][k] = (int)random() % maxNumberInMatrix;
    }

}

void randMatrix2(Matrix *A, int numberLines, int numberColumn){
    (numberColumn) ? (A->width = numberColumn) : (A->width = (int)random() % maxNumberLinesInMatrix);
    (numberLines) ? (A->height = numberLines) : (A->height = (int)random() % maxNumberLinesInMatrix);
    createMatrix(A->height, A->width, A);

    for(int i = 0; i < A->height; i++){
        for(int k = 0; k < A->width; k++)
            (A->matrix)[i][k] = (int)random() % maxNumberInMatrix;
    }

}

void printMatrix(Matrix A){
    for(int i = 0; i < A.height; i++){
        for(int k = 0; k < A.width; k++)
            printf("%7lf ", (A.matrix)[i][k]);
        printf("\n");
    }
    printf("\n");
}

void freeMatrix(Matrix *A){
    for(int i = 0; i < A->height; i++)
        free((A->matrix)[i]);
    free(A->matrix);
}

double Line_x_Column(Matrix A, Matrix B, int line, int column){
    double result = 0;
    for(int i = 0; i < A.width; i++)
        result += (A.matrix)[line][i] * (B.matrix)[i][column];
    return result;
}

long multMatrix(Matrix A, Matrix B, Matrix *C){
    struct timespec ts1, ts2;
    char buff1[100], buff2[100];
    timespec_get(&ts1, TIME_UTC);
    strftime(buff1, sizeof buff1, "%D %T", gmtime(&ts1.tv_sec));
    printf("Current time no  thread: %s.%09ld UTC\n", buff1, ts1.tv_nsec);
    //clock_gettime(CLOCK_REALTIME, &ts1);
    //printf("Current time: %09ld\n", ts1.tv_nsec);

    for(int i = 0; i < A.height; i++) {
        for (int k = 0; k < B.width; k++)
            (C->matrix)[i][k] = Line_x_Column(A, B, i, k);
    }
    timespec_get(&ts2, TIME_UTC);
    strftime(buff2, sizeof buff2, "%D %T", gmtime(&ts2.tv_sec));
    printf("Current time no  thread: %s.%09ld UTC\n\n", buff2, ts2.tv_nsec);
    //int sec = (((buff2[15] - '0')*10 + (buff2[16] - '0')) - ((buff1[15] - '0')*10 + (buff1[16] - '0')) + 60) % 60;

    //long delta = (ts2.tv_nsec- ts1.tv_nsec + sec*1000000000);
    //printf("Delta is               :%09ld, sec = %d\n\n", delta, sec);
    //clock_gettime(CLOCK_REALTIME, &ts2);
    //printf("Working time no  threads is %015ld \n\n", ts2.tv_nsec-ts1.tv_nsec);
    //return delta;
}

void* thread(void * info){
    Thread* infoMy = (Thread*)info;
    for (int k = 0; k < infoMy->B.width; k++)
        (infoMy->C->matrix)[infoMy->n][k] = Line_x_Column(infoMy->A, infoMy->B, infoMy->n, k);
};

long multMatrixThreads(Matrix A, Matrix B, Matrix *C){
    int n = A.height;
    Thread info[n];
    for(int i = 0; i < n; i++){
        info[i].A = A;
        info[i].B = B;
        info[i].C = C;
        info[i].n = i;
    }

    int result[n];
    pthread_t thid[n];

    struct timespec ts1, ts2;
    char buff1[100], buff2[100];
    timespec_get(&ts1, TIME_UTC);
    strftime(buff1, sizeof buff1, "%D %T", gmtime(&ts1.tv_sec));
    printf("Current time for thread: %s.%09ld UTC\n", buff1, ts1.tv_nsec);
    //clock_gettime(CLOCK_REALTIME, &ts1);
    //printf("Current time: %09ld\n", ts1.tv_nsec);

    for(int i = 0; i < n; i++)
        result[i] = pthread_create(thid + i, (pthread_attr_t *) NULL, thread, &(info[i]));
    for(int i = 0; i < n; i++)
        pthread_join(thid[i], (void **) NULL);

    timespec_get(&ts2, TIME_UTC);
    strftime(buff2, sizeof buff2, "%D %T", gmtime(&ts2.tv_sec));
    printf("Current time for thread: %s.%09ld UTC\n", buff2, ts2.tv_nsec);
    //int sec = (((buff2[15] - '0')*10 + (buff2[16] - '0')) - ((buff1[15] - '0')*10 + (buff1[16] - '0')) + 60) % 60;

    //long delta = (ts2.tv_nsec- ts1.tv_nsec + sec*1000000000);
    //printf("Delta is               :%09ld, sec = %d\n\n", delta, sec);
    //clock_gettime(CLOCK_REALTIME, &ts2);
    //printf("Working time for threads is %015ld \n\n", ts2.tv_nsec-ts1.tv_nsec);

    //return delta;
}

int main() {
    Matrix A, B, C, D;
    //srand(time(0));
    int numberColumns;
    scanf("%d", &numberColumns);
    //scanMatrix(&A);
    //scanMatrix(&B);
    randMatrix(&A, numberColumns, numberColumns); // 0 - если число столбцов(строк) сделать rand, иначе - число > 0
    randMatrix(&B, numberColumns, numberColumns);
    //printMatrix(A);
    //printMatrix(B);


    if(A.width != B.height){
        printf("Incompatible matrixs!\n");
        return 0;
    }

    createMatrix(A.height, B.width, &C);
    long b = multMatrix(A, B, &C);
    //printMatrix(C);
    //printf("Working time for threads is %f - %f = %f ticks.\n\n", (double)endTime, (double)startTime, (double)(endTime - startTime));


    createMatrix(A.height, B.width, &D);
    long a = multMatrixThreads(A, B, &D);
    //printf("Working time for threads is %f - %f = %f ticks.\n\n", (double)endTime, (double)startTime, (double)(endTime - startTime));
    //printMatrix(D);
    //printf("Acceleration : %lf %ld %ld\n", (double)b/(double)a, b, a);

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);

    return 0;
}
