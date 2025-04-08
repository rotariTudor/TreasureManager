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
}treasure;

typedef struct{
    char *hunt_id;
    treasure *Tarray;
}Hunt;


void add(treasure Tarray, char *huntID){

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

    printf("treasures:\n");
    while((fisier = readdir(dir)) != NULL) {
        if(strcmp(fisier->d_name, ".") == 0 || strcmp(fisier->d_name, "..") == 0)
            continue;

        printf("%s\n", fisier->d_name);
    }

    closedir(dir);
}

void view(char *treasureName){
    char cwd[128];
    getcwd(cwd,sizeof(cwd));

    DIR *dir=opendir(".");
    if (dir == NULL) {
        printf("Folderul nu exista.\n");
        return;
    }

    struct dirent *intrare;
    struct stat info;

    while((intrare=readdir(dir))!=NULL){
        if(strcmp(intrare->d_name, ".") == 0 || strcmp(intrare->d_name, "..") == 0){
            continue;
        }

        stat(intrare->d_name,&info);

        if(S_ISREG(info.st_mode)) {
            if(strcmp(intrare->d_name, treasureName) == 0) {
                printf("A fost gasit treasure-ul in %s/%s\n", cwd, intrare->d_name);
                closedir(dir);
                exit(0);
            }
        } 
        else if(S_ISDIR(info.st_mode)) {
            
            chdir(intrare->d_name);
            view(treasureName);
            chdir("..");  
        }
    }
    closedir(dir);
}

int remTreasure(const char *treasureName) {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("Eroare deschidere director");
        return 0;
    }

    struct dirent *intrare;
    struct stat info;

    while ((intrare = readdir(dir)) != NULL) {
        if (strcmp(intrare->d_name, ".") == 0 || strcmp(intrare->d_name, "..") == 0)
            continue;

        if (stat(intrare->d_name, &info) == -1)
            continue;

        if (S_ISREG(info.st_mode)) {
            if (strcmp(intrare->d_name, treasureName) == 0) {
                if (remove(treasureName) == 0) {
                    printf("S-a sters fisierul %s\n", treasureName);
                    closedir(dir);
                    return 1;
                } else {
                    perror("Eroare la stergere fisier");
                    closedir(dir);
                    return 0;
                }
            }
        } else if (S_ISDIR(info.st_mode)) {
            if (chdir(intrare->d_name) == 0) {
                int gasit = remTreasure(treasureName);
                chdir("..");
                if (gasit) {
                    closedir(dir);
                    return 1;
                }
            }
        }
    }

    closedir(dir);
    return 0;
}



int main(){

    treasure test1={"treasure1","Tudor",{1.2,2.5},25,"clue1"};
    treasure test2={"treasure2","Gabriel",{2.2,3.5},35,"clue2"};
    treasure test3={"treasure3","Flavius",{3.2,4.5},45,"clue3"};

    add(test1,"proiect1");
    add(test2,"proiect2");
    add(test3,"proiect1");

    //list("proiect1");

    //view("treasure2");

    remTreasure("treasure3");
    
    return 0;
}
