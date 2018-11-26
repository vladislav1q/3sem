#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <memory.h>
#include <zconf.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/wait.h>

int maxNumberOfPlates = 5, n = 0, i = 1;
int tableSize = 2;

typedef struct Plate{
    int washTime, dryTime;
} Plate;

void toDo(int semid, int number, int action){
    struct sembuf mybuf;

    mybuf.sem_num = number;
    mybuf.sem_op  = action;
    mybuf.sem_flg = 0;

    semop(semid, &mybuf, 1);
}

void getPlates(Plate* array){
    int maxLengthOfFilePlates = 10000;
    const char* pathToPlates = "plates2.txt";
    Plate dish;

    char string[maxLengthOfFilePlates], *ptr;
    memset(string, ' ', maxLengthOfFilePlates);

    FILE* f = fopen(pathToPlates, "r");
    if(f == NULL)
        exit(-2);

    int length = (int)fread(string, sizeof(char), maxLengthOfFilePlates, f);
    ptr = string;

    if(length == 0)
        return;
    printf("wash time/dry time\n");
    while(1){
        dish.washTime = atoi(ptr);
        ptr = strchr(ptr, (int)' ');
        dish.dryTime = atoi(ptr);
        ptr = strchr(ptr, (int)'\n');


        *(array + n) = dish;
        printf("%6d %6d\n", dish.washTime, dish.dryTime);
        n++;
        if((ptr - string >= length - 2) || (n >= maxNumberOfPlates))
            break;
    }
    printf("end of reading\n");
    fclose(f);
}

Plate* createSharedMemory(){    // создаем стэк на общей памяти
    char* pathname = "main.c";
    key_t key;
    Plate* array;
    int shmid;

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((shmid = shmget(key, (maxNumberOfPlates + 1) * sizeof(Plate), 0666|IPC_CREAT|IPC_EXCL)) < 0){
        if(errno != EEXIST){
            printf("Can\'t create shared memory\n");
            exit(-1);
        } else {
            if((shmid = shmget(key, (maxNumberOfPlates + 1) * sizeof(Plate), 0)) < 0){
                printf("Can\'t find shared memory\n");
                exit(-1);
            }
        }
    }
    if((array = (Plate*)shmat(shmid, NULL, 0)) == (Plate*)(-1)){
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    return array;
};

void areOnTable(Plate* listOfDish){
    printf("On table now:\nwash time/dry time\n");
    for(int i = 1; i < listOfDish[0].dryTime; i++)
        printf("%6d %6d\n", listOfDish[i].washTime, listOfDish[i].dryTime);
}

int main() {
    system("ipcrm -a");
    Plate* listOfDish, *array = calloc(maxNumberOfPlates, sizeof(Plate));
    //Plate a;
    listOfDish = createSharedMemory();
    getPlates(array);

    int   semid, status;
    char pathname[]="main.c";
    key_t key = ftok(pathname, 0);

    if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t create semaphore set\n");
        exit(-1);
    }
    toDo(semid, 0, 1);
    listOfDish[0].dryTime = i;  //используем первый элемент массива как counter
    listOfDish[0].washTime = n;  //и n

    int pid = fork();

    if(pid > 0){                    //WASH
        int pid2 = fork();
        if(pid2 > 0){
            while(1){
                if(n <= 0)
                    break;
                if(listOfDish[0].dryTime > tableSize)
                    continue;
                printf("Started wash: %d %d\n", array[listOfDish[0].washTime-1].washTime, array[listOfDish[0].washTime-1].dryTime);
                sleep(array[listOfDish[0].washTime-1].washTime);

                toDo(semid, 0, -1);
                Plate counter = array[listOfDish[0].washTime-1];
                listOfDish[(listOfDish[0].dryTime)] = array[listOfDish[0].washTime-1];
                listOfDish[0].washTime--;
                (listOfDish[0].dryTime)++;
                toDo(semid, 0, 1);

                printf("I have washed: %d %d\n", counter.washTime, counter.dryTime);
            }
            waitpid(pid2, &status, CLD_EXITED);
        } else if(pid2 == 0){       //DRY
            while(1){
                //sleep(1);
                if((listOfDish[0].dryTime <= 1) && (listOfDish[0].washTime <= 0))
                    exit(0);
                else if((listOfDish[0].dryTime <= 1))
                    continue;


                toDo(semid, 0, -1);
                (listOfDish[0].dryTime)--;
                Plate counter = listOfDish[(listOfDish[0].dryTime)];
                toDo(semid, 0, 1);

                printf("Started  dry: %d %d\n", counter.washTime, counter.dryTime);
                sleep(counter.dryTime);
                printf("I have dryed: %d %d\n", counter.washTime, counter.dryTime);
            }
        }

    } else if(pid == 0){
        for(int i = 0; i < 40; i++){
            printf("\nSecond: %d\n", i);
            areOnTable(listOfDish);
            sleep(1);
        }

    }

    if(shmdt(listOfDish) < 0){
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    free(array);

    return 0;
}