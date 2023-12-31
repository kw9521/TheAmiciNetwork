/// CSCI-243: The Mechanics of Programming Project 2: The Amici Network
/// @file amici.c
/// @author Kunlin Wen, kw9521

#define DELIMS " \t\n"

#include <stdio.h>      // for I/O operations
#include <stdlib.h>     // for dynamic memory

#include "HashADT.h"
#include "processArgs.h"
#include "Support.h"

// run valgrind using valgrind --leak-check=full --show-leak-kinds=all --log-file=errors.txt ./RUN
// valgrind --leak-check=full --show-leak-kinds=all --log-file=errors.txt ./amici < input.1

/// @brief initializes data structure and processes commands from either the command line, a file, or standard input
/// @param argc number of commandline arguments
/// @param argv array of char pointers of commandline arguments
int main(int argc, char *argv[]) {
    HashADT table = ht_create(str_hash, str_equals, print, delete);

    // if first command is ./amici (nothing)
    if (argc <= 1){
        processCommands(true, NULL, &table);

    } else {    
        // if first command is ./amici datafile.txt
        // read from file

        // Should the command line contain more than one argument after the command name itself, print a usage message 
        // of the following form to stderr and exit with the status EXIT_FAILURE
        if(argc >=3){
            fprintf(stderr, "usage: amici [ %s ]", argv[1]);
            exit(EXIT_FAILURE);
        } else {
            // otherwise open normally
            FILE *fp = fopen(argv[1], "r");
            if (fp == NULL){    // something wrong with opening
                // error occured
                perror(argv[1]);
                // fclose(fp);
                exit(EXIT_FAILURE);
            } else {
                // can open file 
                processCommands(false, fp, &table);
                fclose(fp);
                // process commmand line inputs after reading file
                processCommands(true, fp, &table);
            } 
        }
    }
}
