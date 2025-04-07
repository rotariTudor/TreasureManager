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


void add(Treasure Tarray, char *huntID){

    int check = chdir(huntID);
    
    if(check==-1){
        printf("Directory-ul %s nu exista.\n",huntID);
        int folderCreate = mkdir(huntID,S_IRWXU | S_IRWXG | S_IRWXO);

        if(folderCreate==0){
            printf("S-a creat directory-ul %s.\n",huntID);
            chdir(huntID);
        }
    }
    else if(check==0){
        printf("S-a deschis directory-ul %s.\n",huntID);
    }

    int fileCreate = open(Tarray.treasure_id, O_RDWR | O_CREAT | O_TRUNC, 0644);
    printf("S-a creat fisierul %s.\n",Tarray.treasure_id);

    write(fileCreate,&Tarray.treasure_id,sizeof(Tarray.treasure_id));

    write(fileCreate,&Tarray.username,sizeof(strlen(Tarray.username)));

    char tempCoord[20];
    sprintf(tempCoord,"%f %f",Tarray.coordinates.latitude, Tarray.coordinates.longitude);
    write(fileCreate,&tempCoord,sizeof(tempCoord));
    
    char temp[20];
    sprintf(temp,"%d",Tarray.value);
    write(fileCreate,&temp,sizeof(strlen(temp)));

    write(fileCreate,&Tarray.clue,sizeof(strlen(Tarray.clue)));

    close(fileCreate);
}

int main(){
    Treasure arr={"treasure1","Tudor",{1.2,2.5},25,"clue"};
    add(arr,"proiect");
    return 0;
}
