/// @file amici.c
/// @author Kunlin Wen, kw9521

#include <stddef.h>     // for size_t
#include <stdio.h>      // for I/O operations
#include <stdlib.h>     // for dynamic memory
#include <errno.h>      // for errno and ENOENT
#include <string.h>     // for strtok and other string stuff

#define   DELIMITERS   " \t\n"          // separate tokens using whitespace
#define _DEFAULT_SOURCE 

typedef struct person_s {
    char *name;                 ///< name of the person
    char *handle;               ///< handle of the person
    struct person_s **friends;  ///< dynamic collection of friends
    size_t friend_count;        ///< current number of friends 
    size_t max_friends;         ///< current limit on friends
} person_t;



int main(int argc, char *argv[]) {
    
    // user entered commands
    if (argc >= 1){
        

    } else {
        // read from file

        // Should the command line contain more than one argument after the command name itself, print a usage message 
        // of the following form to stderr and exit with the status EXIT_FAILURE
        if(argc >=3){
            fprintf(stderr, "usage: amici [%s]", argv[1]);
            exit(EXIT_FAILURE);
        } 

        // otherwise open normally
        FILE *fp = fopen(argv[1], "r");

        if (fp == NULL){
            // means file doesnt exit

            if (errno == ENOENT) { 
                // read commands from stdin
            } else {
                // error occured
                perror("File not openable");

            }

        }
        

    }
    
    char buffer[256];
        while ((fgets(buffer,256, stdin)) != NULL) {
            char *token;

            // first call specifies the buffer to be parsed
            // gets rid of space, tabs and new lines
            token = strtok( buffer, DELIMITERS );   

            while (token != NULL) {

                // do something with this token
                printf( "Token: '%s'\n", token );

                // subsequent calls to parse the same buffer
                token = strtok( NULL, DELIMITERS );

            }
        }

}