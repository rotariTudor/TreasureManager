#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

typedef struct{
    float latitude;
    float longitude;
}GPS;

typedef struct{
    int treasure_id;
    char username[20];
    GPS gps;
    int value;
}Treasure;


void add(int hunt_id){
    char temp[15];
    
    sprintf(temp,"Hunt%d",hunt_id);

    int ret=mkdir(temp,S_IRWXU | S_IRWXG | S_IRWXO);
    
    if(ret!=0){
        printf("Nu s-a putut crea folderul.\n");
    }
    else{
        printf("Folderul a fost creat cu succes.\n");
    }
}

int main(){
    add(001);
    return 0;
}