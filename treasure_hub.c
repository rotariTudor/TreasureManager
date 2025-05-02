#define _POSIX_C_SOURCE 200809L
#include "treasure_manager.h"
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

u_int8_t monitor_on_off = 0;
pid_t monitorpid;
u_int8_t procfin = 0;

void handle_sigchld(int signal){
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {
        monitor_on_off = 0;
        printf("Monitorizare încheiată cu exit status %d.\n", WEXITSTATUS(status));
    }
}

void handle_sigusr2(int signal){
    printf("Semnal SIGUSR2 primit.\n");
    exit(-1);
}

void start_monitor(){
    int pid;
    int opt;

    if(monitor_on_off == 1){
        printf("Monitorul deja rulează.\n");
        return;
    }
    signal(SIGCHLD, handle_sigchld);
    if((pid = fork()) < 0){
        printf("[FORK] Nu s-a putut realiza crearea procesului.\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        if(execl("./monitor", "monitor", NULL)==-1){
            printf("Eroare la execuție");
            exit(EXIT_FAILURE);
        }
    }

    if(pid > 0){
        monitor_on_off = 1;
        monitorpid = pid;

        signal(SIGCHLD, handle_sigchld);

        for(;;){
            printf("Introdu o opțiune:\n1 - List hunts\n2 - List treasures\n3 - View treasure\n4 - Stop monitor\n5 - exit\n");
            scanf("%d", &opt);
            switch(opt){
                case 1:
                    printf("Toate hunturile din directory-ul cu hunturi.\n");
                    list_hunts();
                    break;

                case 2:
                    printf("Introdu numele huntului pentru a fi listat.\n");
                    char temp[15] = "";
                    scanf("%14s", temp);
                    list(temp);
                    break;

                case 3:
                    printf("Introdu huntul in care se afla treasure-ul.\n");
                    char temp1[15] = "";
                    scanf("%14s", temp1);

                    printf("Introdu numele treasure-ului pentru a fi listat.\n");
                    char temp2[15] = "";
                    scanf("%14s", temp2);
                    view(temp1, temp2);
                    break;

                case 4:
                    if(monitor_on_off == 0){
                        printf("Monitorul deja este oprit.\n");
                    } else {
                        printf("Monitorul se opreste...\n");
                        monitor_on_off = 0;
                        sleep(3);
                        kill(monitorpid, SIGUSR2);
                        printf("Monitor oprit.\n");
                    }
                    break;

                case 5:
                    if(monitor_on_off == 1){
                        printf("Monitorul inca ruleaza.\n");
                    } else {
                        procfin = 1;
                        exit(0);
                    }
                    break;
            }
            if(procfin == 1){
                break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    struct sigaction sa;
    sa.sa_handler = handle_sigusr2;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    start_monitor();
    return 0;
}
