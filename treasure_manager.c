#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

typedef enum{
    ADD_TREASURE,
    LIST_TREASURE,
    VIEW_TREASURE,
    REMOVE_TREASURE,
    REMOVE_HUNT
  }Option;

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



treasure exemple[10] = {
    {"TR1", "Alex", {1.2, 2.5}, 25, "CL1"},
    {"TR2", "Gabriel", {2.2, 3.5}, 35, "CL2"},
    {"TR3", "Flavius", {3.2, 4.5}, 45, "CL3"},
    {"TR4", "Andrei", {4.1, 5.3}, 55, "CL4"},
    {"TR5", "Ioana", {5.7, 6.8}, 20, "CL5"},
    {"TR6", "Patricia", {6.4, 7.2}, 40, "CL6"},
    {"TR7", "Mihai", {7.9, 8.1}, 30, "CL7"},
    {"TR8", "Diana", {1.5, 3.6}, 15, "CL8"},
    {"TR9", "Alexandra", {2.8, 1.9}, 60, "CL9"},
    {"TR10", "Vlad", {3.3, 4.4}, 50, "CL10"}
};

void addLog(char *huntID, char *mes){
    chdir(huntID);
    
    char name[64]="logged_";
    char localTime[64]="";
    strcat(name,huntID);
    
    int fd = open(name,O_WRONLY | O_CREAT | O_APPEND, 0644);
    
    time_t now=time(NULL);
    struct tm *local_time=localtime(&now);
    strcat(localTime,asctime(local_time));

    char finalmessage[1024]="";
    strcat(finalmessage,mes);
    strcat(finalmessage,"---> Schimbare facuta la: ");
    strcat(finalmessage,localTime);
    strcat(finalmessage,"**************************************************\n\n");
    
    write(fd,finalmessage,strlen(finalmessage));
    
    close(fd);
}

void add(char *huntID, treasure T) {
    char message[512] = "";
    
    int check = chdir(huntID);
    if (check == -1) {
        snprintf(message + strlen(message), sizeof(message) - strlen(message), "Directory-ul %s nu exista.\n", huntID);
        
        mkdir(huntID, S_IRWXU | S_IRWXG | S_IRWXO);

        snprintf(message + strlen(message), sizeof(message) - strlen(message), "S-a creat directory-ul %s.\n", huntID);

        chdir(huntID);
    } else {
        snprintf(message + strlen(message), sizeof(message) - strlen(message), "S-a deschis directory-ul %s.\n", huntID);
    }

    int fd = open("treasures.bin", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("Eroare la deschiderea fisierului treasures.bin");
        snprintf(message + strlen(message), sizeof(message) - strlen(message), "Eroare la deschiderea fisierului treasures.bin");
        
        chdir("..");
        addLog(huntID, message);
        return;
    }

    write(fd, &T, sizeof(treasure));

    snprintf(message + strlen(message), sizeof(message) - strlen(message), "S-a adaugat treasure-ul %s.\n\n", T.treasure_id);
    
    addLog(huntID, message);
    close(fd);
    chdir("..");
}


void list(char *huntID){
    char cale[64];
    snprintf(cale, sizeof(cale), "%s/treasures.bin", huntID);

    int f=open(cale, O_RDONLY);
    if(f==-1){
        printf("NU s-au gasit nici un treasure sau nu exista directory-ul %s.\n",huntID);
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
    addLog(huntID,"S-a folosit functia de list.\n\n");
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
            printf("Treasure-ul gasit in %s este:\n",dir);
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

int remTreasure(char *dir, const char *treasureName) {
    char message[512]="";
    char path[64];
    snprintf(path, sizeof(path), "%s/treasures.bin", dir);

    int fd = open(path, O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return 2;
    }

    treasure tr;
    off_t read_offset = 0;
    off_t write_offset = 0;
    int found = 0;

    while (read_offset + sizeof(treasure) <= st.st_size) {
        lseek(fd, read_offset, SEEK_SET);
        ssize_t bytes = read(fd, &tr, sizeof(treasure));
        if (bytes != sizeof(treasure)) {
            perror("read");
            close(fd);
            return 3;
        }

        if (strcmp(tr.treasure_id, treasureName) == 0) {
            found = 1; 
        } else {
            lseek(fd, write_offset, SEEK_SET);
            write(fd, &tr, sizeof(treasure));
            write_offset += sizeof(treasure);
        }

        read_offset += sizeof(treasure);
    }

    if (!found) {
        printf("Nu exista treasure-ul in director.\n");
        snprintf(message,sizeof(message),"S-a incercat sa se stearga un treasure inexistent.\n\n");
        
        addLog(dir,message);
        close(fd);
        return 4;
    }

    if (ftruncate(fd, write_offset) == -1) {
        perror("ftruncate");
        close(fd);
        return 5;
    }

    printf("Treasure-ul a fost sters cu succes din director.\n");
    snprintf(message,sizeof(message),"S-a sters %s.\n\n",treasureName);
    
    addLog(dir,message);
    
    putchar('\n');
    close(fd);
    return 0;
}

int remHunt(char *huntName) {
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
    putchar('\n');
}



int main(int argc, char *argv[]){

    if(argc<3){
        printf("Prea putine arg in linia de comanda.\n");
        return 1;
    }

    Option optiune;

    if(!strcmp(argv[1],"--add")) optiune=ADD_TREASURE;
    else if(!strcmp(argv[1],"--list")) optiune=LIST_TREASURE;
    else if(!strcmp(argv[1],"--view")) optiune=VIEW_TREASURE;
    else if(!strcmp(argv[1],"--removet")) optiune=REMOVE_TREASURE;
    else if(!strcmp(argv[1],"--removeh")) optiune=REMOVE_HUNT;

    switch(optiune){
        case ADD_TREASURE:
            if(argc!=4){
                perror("Introdu 4 argumente.\n");
                exit(-1);
            }
            for(int i=0;i<10;i++){
                if(!strcmp(exemple[i].treasure_id,argv[3])){
                    add(argv[2],exemple[i]);
                    return 0;
                }
            }
            printf("Introdu un treasure existent (%s - %s).\n",exemple[0].treasure_id,exemple[9].treasure_id);
            break;

        case LIST_TREASURE:
            if(argc!=3){
                perror("Introdu 3 argumente pentru list treasure.\n");
                exit(-2);
            }
            list(argv[2]);
            break;

        case VIEW_TREASURE:
            if(argc!=4){
                perror("Introdu 4 argumente pentru view treasure.\n");
                exit(-3);
            }
            view(argv[2],argv[3]);
            break;

        case REMOVE_TREASURE:
            if(argc!=4){
                perror("Introdu 4 argumente pentru remove treasure.\n");
                exit(-4);
            }
        remTreasure(argv[2],argv[3]);
        break;

        case REMOVE_HUNT:
        if(argc!=3){
            perror("Introdu 3 argumente pentru remove hunt.\n");
            exit(-4);
        }
        remHunt(argv[2]);
        break;

        default:
            printf("Introdu o optiune valida!.\n");
            break;
    }

    return 0;
}