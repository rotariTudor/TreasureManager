#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
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
    wait(&status);
    monitor_on_off=0;
    printf("Monitor oprit cu exit status: %d.\n",WEXITSTATUS(status));
}

void handle_sigusr2(int signal){
    printf("Semnal SIGUSR2 de oprire a fost primit.\n");
    sleep(3);
    exit(0);
}

void scoreCalc(){
    DIR *dir=opendir(".");
    struct dirent *intr;

    if(!dir){
        printf("Eroare la deschiderea directorului.\n");
        return;
    }

    printf("Rezultatele scorurilor pentru hunt-uri:\n");

    
    while((intr=readdir(dir))!=NULL){
        struct stat st;

        if(strcmp(intr->d_name,".")!=0 && strcmp(intr->d_name,"..")!=0 && strcmp(intr->d_name,".git")!=0 && strcmp(intr->d_name,".vscode")!=0 && stat(intr->d_name,&st)==0 && S_ISDIR(st.st_mode)){
            int pipefd[2];

            if(pipe(pipefd)==-1){
                printf("Eroare la crearea pipe-ului.\n");
                continue;
            }

            pid_t pid=fork();
            if(pid==-1){
                printf("Eroare la fork.\n");
                close(pipefd[0]);
                close(pipefd[1]);
                continue;
            }

            if(pid==0){
                close(pipefd[0]);
                dup2(pipefd[1],STDOUT_FILENO);
                close(pipefd[1]);
                
                char *args[]={"./score_calculator",intr->d_name,NULL};
                execv(args[0],args);
                printf("Eroare la execv.\n");
                exit(1);
            }else{
                close(pipefd[1]);
                printf("Hunt: %s\n",intr->d_name);

                char buffer[256];
                ssize_t bytes;
                while ((bytes = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytes] = '\0';
                    printf("%s", buffer);
                }
                close(pipefd[0]);
                waitpid(pid, NULL, 0);
                printf("------------------------------------------------\n");
            }
        }
    }
    closedir(dir);
}

void start_monitor(){
    int pid;
    int opt;

    if((pid = fork()) < 0){
        printf("[FORK] Nu s-a putut realiza crearea procesului.\n");
        exit(EXIT_FAILURE);
    }

    if(pid > 0){
        monitor_on_off = 1;
        monitorpid = pid;
        printf("S-a creat un proces cu ID-ul: %d.\n",monitorpid);
        printf("***************************\n");

        for(;;){
            printf("Introdu o opțiune:\n1 - List hunts\n2 - List treasures\n3 - View treasure\n4 - Score calculator\n5 - Stop monitor\n6 - exit\n");
            printf("***************************\n");
            scanf("%d", &opt);
            getchar();

            switch(opt){
                case 1:
                    printf("Toate hunturile din directory-ul cu hunturi:\n");
                    list_hunts();
                    printf("***************************\n");
                    break;

                case 2:
                    printf("Introdu numele huntului pentru a fi listat.\n");
                    char temp[15] = "";
                    scanf("%14s", temp);
                    list(temp);
                    printf("***************************\n");
                    break;

                case 3:
                    printf("Introdu huntul in care se afla treasure-ul.\n");
                    char temp1[15] = "";
                    scanf("%14s", temp1);

                    printf("Introdu numele treasure-ului pentru a fi listat.\n");
                    char temp2[15] = "";
                    scanf("%14s", temp2);
                    view(temp1, temp2);
                    printf("***************************\n");
                    break;

                case 4:
                    scoreCalc();
                    printf("***************************\n");
                    break;
                    
                case 5:
                    if(monitor_on_off == 0){
                        printf("Monitorul deja este oprit.\n");
                        printf("***************************\n");
                    } else {
                        printf("Monitorul se opreste...\n");
                        monitor_on_off = 0;
                        sleep(3); // am folosit sleep deoarece nu-mi merge usleep, include-urile de mai sus tot au fost folosit pt a nu primi warning-uri in terminal la usleep dar nu a functionat
                        kill(monitorpid, SIGUSR2);
                        printf("Monitor oprit.\n");
                        printf("***************************\n");
                    }
                    break;

                case 6:
                    if(monitor_on_off == 1){
                        printf("Monitorul inca ruleaza.\n");
                        printf("***************************\n");
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
