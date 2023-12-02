/// skeleton layout for processArgs.c
// defines the functions
/// @file processArgs.h
/// @author Kunlin Wen, kw9521

#ifndef PROCESS_ARGS_H
#define PROCESS_ARGS_H

#define DELIMS " \t\n"
#include "HashADT.h"
#include "Support.h"

typedef struct person_s {
    char *firstName;            // name of the person
    char *lastName;
    char *handle;               // handle of the person
    struct person_s **friends;  // dynamic collection of friends
    size_t numOfFriends;        // number of friendships 
    size_t maxFriends;    // current limit on friends, need if ur using dynamic array that doubles in size everytme the array is full
} person_t;

person_t* makePerson(char* firstName, char* lastName, char* handle);
bool checkValidName(char* name);
bool checkHandle(char* handle);
bool checkCommand(char* token);
bool checkIfHandleExists(const HashADT table, const char *handle);
bool notSamePerson(const char *handle1, const char *handle2);
bool areTheyFriendsAlr(const HashADT table, const char *handle1, const char *handle2);
void addFriends(const HashADT table, const char *handle1, const char *handle2);
void freeAllValuesInHashTable(HashADT table);
void initSystem(HashADT* table);
void printCase(const HashADT table, const char *handle);
void quitCase(HashADT table);
void sizeCase(const HashADT table, const char *handle);
void stats();
void removeFriend(person_t* person, const char* handle);
void unfriend(const HashADT table, const char *handle1, const char *handle2);
void delete( void *key, void *value);
void print( const void *key, const void *value );
void processCommands(bool isStdin, FILE *fp, HashADT* table);

#endif