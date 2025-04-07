#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct{
    float latitude;
    float longitude;
}GPS;

typedef struct{
    char treasure_id[10];
    char username[10];
    GPS coordinates;
    int value;
    char clue[10];
}Treasure;

typedef struct{
    char *hunt_id;
    Treasure *Tarray;
}Hunt;


void add(Treasure Tarray,char *huntID){
    int folder = mkdir(huntID,S_IRWXU | S_IRWXG | S_IRWXO);
}

int main(){
    return 0;
}
