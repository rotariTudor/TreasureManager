#include "treasure_manager.c"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <signal.h>

u_int8_t monitor_on_off=0;
pid_t monitorpid;

void handle_sigchld(int signal){
    int status;
    wait(&status);
    monitor_on_off=0;
    printf("Monitorizare încheiată cu exit status %d.\n", WEXITSTATUS(status));
}

void handle_sigusr2(int signal){
    printf("Semnal SIGUSR2 primit.\n");
    usleep(200000);
    exit(-1);
}

void start_monitor(){
    int pid;
    int opt;

    if(monitor_on_off==1){
        printf("Monitorul deja rulează.\n");
        return;
    }

    if((pid=fork())<0){
        printf("[FORK] Nu s-a putut realiza crearea procesului.\n");
        exit(EXIT_FAILURE);
    }

    if(pid==0){
        execl("./monitor","monitor",NULL);
        printf("Eroare la execuție");
        exit(EXIT_FAILURE);
    }

    if(pid>0){
        monitor_on_off=1;
        monitorpid=pid;

        signal(SIGCHLD,handle_sigchld);

        for(;;){
            printf("Introdu o opțiune:\n1 - List hunts\n2 - List treasures\n3 - View treasure\n4 - Stop monitor\n5 - exit\n");
            scanf("%d",&opt);
            switch(opt){
                case 1:
                    
            }
        }
    }
}

