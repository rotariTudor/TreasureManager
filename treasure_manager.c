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

void add(treasure T, char *huntID) {
    int check = chdir(huntID);
    if(check == -1) {
        printf("Directory-ul %s nu exista.\n", huntID);
        mkdir(huntID, S_IRWXU | S_IRWXG | S_IRWXO);
        printf("S-a creat directory-ul %s.\n", huntID);
        chdir(huntID);
    } else {
        printf("S-a deschis directory-ul %s.\n", huntID);
    }

    int fd = open("treasures.bin", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("Eroare la deschiderea fisierului treasures.bin");
        chdir("..");
        return;
    }

    write(fd, &T, sizeof(treasure));
    printf("S-a adaugat treasure-ul %s.\n", T.treasure_id);
    putchar('\n');
    close(fd);
    chdir("..");
}


void list(char *huntID){
    char cale[64];
    snprintf(cale, sizeof(cale), "%s/treasures.bin", huntID);

    int f=open(cale, O_RDONLY);
    if(f==-1){
        printf("NU s-au gasit nici un treasure sau nu exista.\n");
        return;
    }

    struct stat st;
    if(fstat(f,&st)==-1){
        perror("fstat return.\n");
        close(f);
        return;
    }

    printf("Hunt: %s\n",huntID);
    printf("Treasure file size: %ld bytes\n",st.st_size);
    printf("Last modified: %s", ctime(&st.st_mtime));

    treasure tr;
    off_t offset=0;
    int gasit=0;
    int it=0;

    while(offset+sizeof(treasure)<=st.st_size){
        ssize_t bytes = read(f,&tr,sizeof(treasure));
        if(bytes!=sizeof(treasure)){
            if(bytes==-1){
                perror("read fault.\n");
            }
            break;
        }
        printf("----------\n");
        printf("Treasure %d:\nTreasure ID: %s\nUsername: %s\nCoordinates: %.2f %.2f\nValue: %d\nClue: %s\n",++it,tr.treasure_id,tr.username,tr.coordinates.latitude,tr.coordinates.longitude,tr.value,tr.clue);
        offset+=bytes;
        gasit=1;
    }
    if(!gasit) printf("Nu s-a gasit nici un treasure.\n");
    putchar('\n');
    close(f);
}

void view(char *dir, char *treasureName){
    char cale[64];
    snprintf(cale, sizeof(cale), "%s/treasures.bin", dir);

    int f=open(cale, O_RDONLY);
    if(f==-1){
        printf("NU s-au gasit nici un treasure sau nu exista.\n");
        return;
    }

    struct stat st;
    if(fstat(f,&st)==-1){
        perror("fstat return.\n");
        close(f);
        return;
    }

    treasure tr;
    off_t offset=0;
    int gasit=0;

    while (offset + sizeof(treasure) <= st.st_size) {
        ssize_t bytes = read(f, &tr, sizeof(treasure));
        if (bytes != sizeof(treasure)) {
            if (bytes == -1) {
                perror("read");
            }
            break;
        }

        if (strcmp(tr.treasure_id, treasureName) == 0) {
            printf("Treasure-ul gasit este:\n");
            printf("Treasure ID: %s\nUsername: %s\nCoordinates: %.2f %.2f\nValue: %d\nClue: %s\n",tr.treasure_id,tr.username,tr.coordinates.latitude,tr.coordinates.longitude,tr.value,tr.clue);
            gasit = 1;
            break;
        }

        offset += bytes;
    }

    if(!gasit){
        printf("Nu a fost gasit nici un treasure cu Treasure ID: %s.\n",tr.treasure_id);
    }
    putchar('\n');
    close(f);
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
                    printf("S-a sters fisierul %s.\n", treasureName);
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

int remFolder(char *huntName) {
    DIR *dir = opendir(huntName);
    if (dir == NULL) {
        perror("Eroare deschidere director");
        return 1;
    }

    struct dirent *fisier;
    char cale[512];

    while ((fisier = readdir(dir)) != NULL) {
        if (strcmp(fisier->d_name, ".") == 0 || strcmp(fisier->d_name, "..") == 0)
            continue;

        snprintf(cale, sizeof(cale), "%s/%s", huntName, fisier->d_name);

        if (remove(cale) == 0) {
            printf("Sters: %s\n", fisier->d_name);
        } else {
            perror("Eroare stergere fisier");
            closedir(dir);
            return 2;
        }
    }

    closedir(dir);

    if (rmdir(huntName) == 0) {
        printf("S-a sters folderul %s\n", huntName);
        return 0;
    } else {
        perror("Eroare la stergere folder.\n");
        return 3;
    }
}



int main(){

    treasure test1={"treasure1","Tudor",{1.2,2.5},25,"clue1"};
    treasure test2={"treasure2","Gabriel",{2.2,3.5},35,"clue2"};
    treasure test3={"treasure3","Flavius",{3.2,4.5},45,"clue3"};

    add(test1,"proiect1");
    add(test2,"proiect2");
    add(test3,"proiect1");

    list("proiect1");

    view("proiect2","treasure2");

    //remTreasure("treasure3");
    
    remFolder("proiect1");

    return 0;
}
