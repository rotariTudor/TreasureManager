#include "treasure_manager.c"

int main(int argc, char *argv[]){

    if(argc<2){
        printf("Prea putine arg in linia de comanda.\n");
        return 1;
    }

    Option optiune;

    if(!strcmp(argv[1],"--add")) optiune=ADD_TREASURE;
    else if(!strcmp(argv[1],"--list_t")) optiune=LIST_TREASURE;
    else if(!strcmp(argv[1],"--view")) optiune=VIEW_TREASURE;
    else if(!strcmp(argv[1],"--remove_t")) optiune=REMOVE_TREASURE;
    else if(!strcmp(argv[1],"--remove_h")) optiune=REMOVE_HUNT;
    else if(!strcmp(argv[1],"--list_h")) optiune=LIST_HUNTS;
    else{
        printf("comanda necunoscuta.\nintrodu --add, --list_t, --view, --remove_t, --remove_h, -- list,h\n");
        return 1;
    }

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
            exit(-5);
        }
        remHunt(argv[2]);
        break;

        case LIST_HUNTS:
            if(argc!=2){
                perror("Introdu 2 argumente pentru list hunt.\n");
                exit(-6);
            }
            list_hunts();
            break;

        default:
            printf("Introdu o optiune valida!.\n");
            break;
    }
    return 0;
}