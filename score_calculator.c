#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct{
    char treasure_id[15];
    char username[15];
    struct {
        float latitude;
        float longitude;
    } coordinates;
    int value;
    char clue[15];
}treasure;

typedef struct{
    char username[15];
    int total_score;
}user_score;

int main(int argc, char *argv[]){
    if(argc!=2){
        printf("Introdu 2 argumente in linia de comanda %s - <hunt_dir>\n",argv[0]);
        return 1;
    }

    char path[256];
    
    snprintf(path,sizeof(path),"%s/treasures.bin",argv[1]);

    int fd = open(path, O_RDONLY);
    if(fd==-1){
        printf("Eroare la deschidere fisier din calea %s.\n",path);
        return 1;
    }

    user_score users[100];
    int count=0;
    treasure tr;

    while(read(fd,&tr,sizeof(treasure))==sizeof(treasure)){
        int found=0;
        for(int i=0;i<count;i++){
            if(strcmp(users[i].username,tr.username)==0){
                users[i].total_score+=tr.value;
                found=1;
                break;
            }
        }
        if(!found){
            strncpy(users[count].username,tr.username,sizeof(tr.username));
            users[count].total_score=tr.value;
            count++;
        }
    }
    close(fd);

    for(int i=0;i<count;i++){
        printf("User: %s, Total Score: %d\n",users[i].username,users[i].total_score);
    }
}