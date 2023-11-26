/// @file processArgs
/// @author Kunlin Wen, kw9521

#define DELIMITERS   " \t\n"        // separate tokens using whitespace
#define _DEFAULT_SOURCE             // must be before all #include to be able to use getline()

#include <stddef.h>     // for size_t
#include <stdio.h>      // for I/O operations
#include <stdlib.h>     // for dynamic memory
#include <errno.h>      // for errno and ENOENT
#include <string.h>     // for strtok and other string stuff
#include <stdbool.h>
#include <ctype.h>      // for isalpha
#include "HashADT.h"


bool checkValidName(char* name) {

    // check if first letters is an alphabet character
    if (name == NULL || !isalpha(name[0])) {
        return false;
    }

    // checks if alphabetic characters (uppercase and lowercase) along with the apostrophe ('\'') and hyphen ('-') characters
    for (int i = 1; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != '\'' && name[i] != '-') {
            return false;
        }
    }

    return true;
}

bool checkHandle(char *handle) {

    // check if first letters is an alphabet character
    if (handle == NULL || !isalpha(handle[0])) {
        return false;
    }

    // checks if handle consist of alphanumeric characters 
    for (int i = 1; handle[i] != '\0'; i++) {
        if (!isalnum(handle[i])) {
            return false;
        }
    }

    return true;
}


void processCommands(bool isStdin, FILE *fp){
    char buffer[256];
    while ((isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp)) != NULL) {
        char *token;

        // first call specifies the buffer to be parsed
        // gets rid of space, tabs and new lines
        token = strtok( buffer, DELIMITERS );   

        while (token != NULL) {

            if (strcmp(token, "add")){
                // add  first-name  last-name  handle              
            
                char * firstName = strtok( NULL, DELIMITERS );
                char * lastName = strtok( NULL, DELIMITERS );
                char * handle = strtok( NULL, DELIMITERS );
                char * extras = strtok( NULL, DELIMITERS );

                // checks firstName, lastName, handle is NOT NULL, checks of extras IS NULL
                // if any of these are true, it means there is too few args or too much args
                if (firstName == NULL || lastName == NULL|| handle == NULL || !(extras == NULL)) {
                    printf("error: usage: add first-name last-name handle");
                }

                if (!checkValidName(firstName)){
                    printf("error: argument \"%s\" is invalid", firstName);
                } else if (!checkValidName(lastName)){
                    printf("error: argument \"%s\" is invalid", lastName);
                } else if (!checkHandle){
                    printf("error: argument \"%s\" is invalid", handle);
                } else {
                    // all 3 are valid... call the add funtion
                }

                
            } else if (strcmp(token, "friend")){
                // friend  handle1  handle2

                char * handle1 = strtok( NULL, DELIMITERS );
                char * handle2 = strtok( NULL, DELIMITERS );
                char * extras = strtok( NULL, DELIMITERS );
                
                // checks to make sure handle1 and handle1 are valid and that extras is NULL
                // if any of these are false, there are too few or too many args 
                if(handle1 == NULL || handle2 == NULL|| !(extras == NULL)){
                    printf("error: usage: friend  handle1  handle2");
                } 

                // check if handle1 and handle2 are part of the database!!!!!
                // if not, print... printf("error: handle "%s" is unknown", handle1/handle2);


                if (checkHandle(handle1) && checkHandle(handle2)){
                    
                    // TODO
                    // handle1 and handle2 are VALID, are alphanumerical n starts with letter...call the friend command
                    // TODO

                }
            
                
            } else if (strcmp(token, "init")){
                // init

                char * extras = strtok( NULL, DELIMITERS );

                // if there are args after "init" command, its an error
                if (!(extras == NULL)){
                    printf("error: usage: init");
                } 

                // otherwise, Delete the entire contents of the system, releasing all dynamic storage
                // Re-initialize to an empty network, print the following message, and then loop to print a new prompt
                // message: System re-initialized
                
                
            } else if (strcmp(token, "print")){
                // print handle

                char * handle = strtok( NULL, DELIMITERS );
                char * extras = strtok( NULL, DELIMITERS );

                // check for valid amount of arguments
                if (handle == NULL || !(extras == NULL)){
                    printf("error: usage: print handle");
                }

                if (checkHandle(handle)){
                    
                    // handle is valid
                    // Use the handle to look up the person in the table and print the person's name, handle, and the person's friend list
                    // handle (fname lname) has no friends
                    // handle (fname lname) has N friend
                    // (\t)handle (fname lname)
                    // eg: abe (A Lincoln) has 3 friends
	                //          anthony (A Blinken)
                
                } 
                
                
            } else if (strcmp(token, "quit")){
                // quit

                char * extras = strtok( NULL, DELIMITERS );

                // if there are args after "quit" command, its an error
                if (!(extras == NULL)){
                    printf("error: usage: quit");
                } 

                // otherwise, Clean up and delete all dynamic memory, and terminate with the EXIT_SUCCESS termination code
                
            } else if (strcmp(token, "size")){
                // size handle
                char * handle = strtok( NULL, DELIMITERS );
                char * extras = strtok( NULL, DELIMITERS );
                
                // check for valid amount of arguments
                if (handle == NULL || !(extras == NULL)){
                    printf("error: usage: size handle");
                }

                // otherwise...
                // Compute and print the size of (i.e., the number of entries in) the friend list for that user, using one of the following formats according to the number of friends
                // handle (fname lname) has no friends
                // handle (fname lname) has 1 friend
                // handle (fname lname) has N friends

                
            } else if (strcmp(token, "stats")){
                // stats
                
                char * extras = strtok( NULL, DELIMITERS );

                // if there are args after "quit" command, its an error
                if (!(extras == NULL)){
                    printf("error: usage: stats");
                } 

                // otherwise...
                // Print two values: the size of the network and the number of friendships
                // network size is the total number of persons in the system
                // number of friendships is the number created since the start
                // Statistics: X people, Y friendships
    
                
            } else if (strcmp(token, "unfriend")){
                // unfriend  handle1  handle2

                char * handle1 = strtok( NULL, DELIMITERS );
                char * handle2 = strtok( NULL, DELIMITERS );
                char * extras = strtok( NULL, DELIMITERS );
                
                // checks to make sure handle1 and handle1 are valid and that extras is NULL
                // if any of these are false, there are too few or too many args 
                if(handle1 == NULL || handle2 == NULL|| !(extras == NULL)){
                    printf("error: usage: unfriend  handle1  handle2");
                } 

                // check if handle1 and handle2 are even friends in the first place!!!!!
                // if not, print... printf("%s and %s are not friends.", handle1, handle2);


                if (checkHandle(handle1) && checkHandle(handle2)){
                    
                    // handle1 and handle2 are VALID, are alphanumerical n starts with letter
                    // call unfriend func

                }
                
            } else {        
                // command does not meet this: which consist solely of lowercase alphabetic characters. Commands appear only as the first token on an input line

            
            }
            // subsequent calls to parse the same buffer
            token = strtok( NULL, DELIMITERS );
    }
}