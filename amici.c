/// @file amici.c
/// @author Kunlin Wen, kw9521

#define DELIMITERS   " \t\n"        // separate tokens using whitespace
#define _DEFAULT_SOURCE             // must be before all #include to be able to use getline()

#include <stddef.h>     // for size_t
#include <stdio.h>      // for I/O operations
#include <stdlib.h>     // for dynamic memory
#include <errno.h>      // for errno and ENOENT
#include <string.h>     // for strtok and other string stuff
#include "HashADT.h"

typedef struct person_s {
    char *firstName;            // name of the person
    char *lastName;
    char *handle;               // handle of the person
    struct person_s **friends;  // dynamic collection of friends
    size_t numOfFriends;        // number of friendships 
    size_t maxFriends;         // current limit on friends, need if ur using dynamic array that doubles in size everytme the array is full
} person_t;

person_t* makePerson(char* firstName, char* lastName, char* handle){

    person_t * person = (person_t*)malloc(sizeof(person_t));

    // check if memeory was correctly allocated
    assert(person != NULL);

    person -> firstName = firstName;
    person -> lastName = lastName;
    person -> handle = handle;
    person -> friends = (person_t**)malloc((person -> max_friends)* sizeof(person_t*));
    person -> numOfFriends = 0;
    person -> maxFriends = 16;

}

int main(int argc, char *argv[]) {
    person_t *newPerson = (person_t *)malloc(sizeof(person_t));

    // if first command is ./amici (nothing)
    if (argc <= 1){
        

    } else {    // if first command is ./amici datafile.txt
        // read from file

        // Should the command line contain more than one argument after the command name itself, print a usage message 
        // of the following form to stderr and exit with the status EXIT_FAILURE
        if(argc >=3){
            free(newPerson);
            fprintf(stderr, "usage: amici [ %s ]", argv[1]);
            exit(EXIT_FAILURE);
        } 

        // otherwise open normally
        FILE *fp = fopen(argv[1], "r");
        char buffer[256];
        if (fp == NULL){    // something wrong with opening
            if (errno == ENOENT) { 
                // means file doesnt exit
                // read commands from stdin

                // TBD TBD TBD 

            } else {
                // error occured
                free(buffer);
                free(newPerson);
                perror("File not openable");
                exit(EXIT_FAILURE);
            }

        } else {
            // can open file 

            // initialize everything in hashtable 
            // check if memory allocating for hashtable worked
            assert(newPerson != NULL);
            newPerson -> numOfFriendships = 0;
            newPerson -> totalNetworks = 0;
            // newPerson -> max_friends = 100; //replace

            processCommands(false, fp, argc);

        } 
    }
}