#ifndef PROCESS_ARGS_H
#define PROCESS_ARGS_H

#include <stddef.h>     // for size_t
#include <stdio.h>      // for I/O operations
#include <stdlib.h>     // for dynamic memory
#include <errno.h>      // for errno and ENOENT
#include <string.h>     // for strtok and other string stuff
#include <stdbool.h>
#include <ctype.h>      // for isalpha
#include <assert.h>     // for assert()
#include "HashADT.h"
#include "Support.h"

typedef struct person_s *person_t;  
person_t makePerson(char* firstName, char* lastName, char* handle);
bool checkValidName(char* name);
bool checkHandle(char* handle);
bool checkIfHandleExists(const HashADT table, const char *handle);
void freePersonS(person_t person);
bool areTheyFriendsAlr(const HashADT table, const char *handle1, const char *handle2);
void addFriends(const HashADT table, const char *handle1, const char *handle2);
void freeAllValuesInHashTable(HashADT table);
void initSystem(HashADT* table);
void printCase(const HashADT table, const char *handle);
void quitCase(HashADT table);
void sizeCase(const HashADT table, const char *handle);
void stats();
void removeFriend(person_t person, const char* handle);
void unfriend(const HashADT table, const char *handle1, const char *handle2);
void delete( void *key, void *value);
void print( const void *key, const void *value );
void processCommands(bool isStdin, FILE *fp, HashADT* table);

#endif