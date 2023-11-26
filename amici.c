/// @file amici.c
/// @author Kunlin Wen, kw9521

#define DELIMITERS   " \t\n"        // separate tokens using whitespace
#define _DEFAULT_SOURCE             // must be before all #include to be able to use getline()

#include <stddef.h>     // for size_t
#include <stdio.h>      // for I/O operations
#include <stdlib.h>     // for dynamic memory
#include <stdbool.h>
#include <errno.h>      // for errno and ENOENT
#include <string.h>     // for strtok and other string stuff
#include "HashADT.h"

int main(int argc, char *argv[]) {
    // if first command is ./amici (nothing)
    if (argc <= 1){
        processCommands(true, NULL);

    } else {    // if first command is ./amici datafile.txt
        // read from file

        // Should the command line contain more than one argument after the command name itself, print a usage message 
        // of the following form to stderr and exit with the status EXIT_FAILURE
        if(argc >=3){
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
                processCommands(true, NULL);

            } else {
                // error occured
                free(buffer);
                perror("File not openable");
                exit(EXIT_FAILURE);
            }

        } else {
            // can open file 
            processCommands(false, fp);

        } 
    }
}