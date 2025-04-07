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
    char treasure_id[15];
    char username[15];
    GPS coordinates;
    int value;
    char clue[15];
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

    int fisierCreate = open(Tarray.treasure_id, O_RDWR | O_CREAT | O_APPEND, 0644);
    printf("S-a creat fisierul %s.\n\n",Tarray.treasure_id);

    write(fisierCreate,&Tarray.treasure_id,sizeof(Tarray.treasure_id));

    write(fisierCreate, Tarray.username, strlen(Tarray.username));

    char tempCoord[20];
    sprintf(tempCoord,"%f %f",Tarray.coordinates.latitude, Tarray.coordinates.longitude);
    write(fisierCreate,&tempCoord,sizeof(tempCoord));
    
    char temp[20];
    sprintf(temp,"%d",Tarray.value);
    write(fisierCreate,&temp,sizeof(strlen(temp)));

    write(fisierCreate,&Tarray.clue,sizeof(strlen(Tarray.clue)));
    chdir("..");

    close(fisierCreate);
}

void list(char *huntID) {

    DIR *dir = opendir(huntID);
    if (dir == NULL) {
        printf("Folderul %s nu exista.\n", huntID);
        return;
    }

    printf("Hunt: %s\n", huntID);

    struct dirent *fisier;
    struct stat info;

    char caleFisier[512];
    off_t dimensiuneaTotala = 0;
    time_t ultimaModificare = 0;

    while((fisier = readdir(dir)) != NULL) {
        if(strcmp(fisier->d_name, ".") == 0 || strcmp(fisier->d_name, "..") == 0)
            continue;

        sprintf(caleFisier, "%s/%s", huntID, fisier->d_name);

        if(stat(caleFisier, &info) == -1)
            continue;

        dimensiuneaTotala += info.st_size;
        if(info.st_mtime > ultimaModificare)
            ultimaModificare = info.st_mtime;
    }

    printf("Marimea totala a folderului: %ld bytes\n", dimensiuneaTotala);
    printf("Ultima Modificare: %s", ctime(&ultimaModificare));

    rewinddir(dir); 

    printf("Treasures:\n");
    while((fisier = readdir(dir)) != NULL) {
        if(strcmp(fisier->d_name, ".") == 0 || strcmp(fisier->d_name, "..") == 0)
            continue;

        printf("%s\n", fisier->d_name);
    }

    closedir(dir);
}

int main(){

    Treasure test1={"treasure1","Tudor",{1.2,2.5},25,"clue1"};
    Treasure test2={"treasure2","Gabriel",{2.2,3.5},35,"clue2"};
    Treasure test3={"treasure3","Flavius",{3.2,4.5},45,"clue3"};

    add(test1,"proiect1");
    add(test2,"proiect2");
    add(test3,"proiect1");

    list("proiect1");

    return 0;
}
