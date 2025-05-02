#ifndef TREASURE_MANAGER_H
#define TREASURE_MANAGER_H

#include <stdio.h>  
#include <stdlib.h>

typedef enum {
    ADD_TREASURE,
    LIST_TREASURE,
    VIEW_TREASURE,
    REMOVE_TREASURE,
    REMOVE_HUNT,
    LIST_HUNTS
} Option;

typedef struct {
    float latitude;
    float longitude;
} GPS;

typedef struct {
    char treasure_id[15];
    char username[15];
    GPS coordinates;
    int value;
    char clue[15];
} treasure;

// Function prototypes
void addLog(char *huntID, char *mes);
void add(char *huntID, treasure T);
void list(char *huntID);
void view(char *dir, char *treasureName);
int remTreasure(char *dir, const char *treasureName);
int remHunt(char *huntName);
void list_hunts();

#endif 
