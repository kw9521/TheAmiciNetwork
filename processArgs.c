/// CSCI-243: The Mechanics of Programming Project 2: The Amici Network
/// this file processes the file or commandline arguments and calculates whatever is being read/passed in
/// @file processArgs.c
/// @author Kunlin Wen, kw9521

#define _DEFAULT_SOURCE             // must be before all #include to be able to use getline()
#define DELIMS " \t\n"

#include <stddef.h>     // for size_t
#include <stdio.h>      // for I/O operations
#include <stdlib.h>     // for dynamic memory
#include <errno.h>      // for errno and ENOENT
#include <string.h>     // for strtok and other string stuff
#include <stdbool.h>
#include <ctype.h>      // for isalpha()
#include <assert.h>     // for assert()

#include "HashADT.h"
#include "Support.h"
#include "processArgs.h"

size_t numOfPplInNetwork = 0;
size_t numOfFriendShips = 0;

/// Creates a new person_t structure and initializes it with given names and handle
/// Allocates memory for the person and their friends list
/// @param firstName first name of the person
/// @param lastName last name of the person
/// @param handle unique handle to identify the person
person_t* makePerson(char* firstName, char* lastName, char* handle){
    person_t * person = (person_t*)malloc(sizeof(person_t));

    // check if memeory was correctly allocated
    assert(person != NULL && firstName != NULL && lastName !=NULL && handle !=NULL);

    person -> maxFriends = 10;  
    person -> firstName = firstName;
    person -> lastName = lastName;
    person -> handle = handle;

    person-> friends = (person_t**)malloc((person->maxFriends)*sizeof(person_t*));
    assert(person-> friends != NULL);
    person -> numOfFriends = 0;

    numOfPplInNetwork++;
    return person;
}

/// Checks if the given name is valid. 
/// A valid name starts with an alphabetic character and can contain alphabetic characters, apostrophes, and hyphens
/// @param name name to be checked
/// @return true if name is valid, false otherwise
bool checkValidName(char* name) {
    if(name == NULL){
        return false;
    }

    // check if first letters is an alphabet character
    if (name == NULL || !isalpha(name[0])) {
        printf("error: argument \"%s\" is invalid\n", name);
        return false;
    }

    // checks if alphabetic characters (uppercase and lowercase) along with the apostrophe ('\'') and hyphen ('-') characters
    for (int i = 1; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != '\'' && name[i] != '-') {
            printf("error: argument \"%s\" is invalid\n", name);
            return false;
        }
    }

    return true;
}

/// Checks if the given handle is valid
/// A valid handle starts with an alphabetic character and contains alphanumeric characters
/// @param handle handle to be checked
/// @return true if handle is valid, false otherwise
bool checkHandle(char* handle) {

    if(handle == NULL){
        return false;
    }

    // check if first letters is an alphabet character
    if (handle == NULL || !isalpha(handle[0])) {
        printf("error: argument \"%s\" is invalid\n", handle);
        return false;
    }

    // checks if handle consist of alphanumeric characters 
    for (int i = 1; handle[i] != '\0'; i++) {
        if (!isalnum(handle[i])) {
            printf("error: argument \"%s\" is invalid\n", handle);
            return false;
        }
    }

    return true;
}

/// Validates if a command consists only of lowercase letters
/// @param token command to be checked
/// @return true if entire string is only lowercase letters, false otherwise
bool checkCommand(char* token){
    if (token == NULL) {
        return false; 
    }

    for (int i = 0; token[i] != '\0'; i++) {
        if (!islower(token[i])) {
            return false; // Return false if any character is not lowercase
        }
    }

    return true; // All characters are lowercase
}

/// Checks if a given handle exists in the hash table
/// @param table the hashtable to search
/// @param handle handle to be checked
/// @return true if handle exists in table, false otherwise
bool checkIfHandleExists(const HashADT table, const char *handle){

    if (ht_has(table, handle)) {
        return true;
    } else {
        printf("error: handle \"%s\" is unknown\n", handle);
        return false; 
    }
}

/// checks if handle1 and handle2 are the same person
/// @param handle1 first handle to be compared with
/// @param handle2 second handle to be compared to
/// @return false if handle1 and handle2 are the same, true if it is NOT the same person
bool notSamePerson(const char *handle1, const char *handle2){
    if (str_equals(handle1, handle2)){
        printf("error: \"%s\" and \"%s\" are the same person\n", handle1, handle2);
        return false;
    } else {
        return true; 
    }
}

/// Checks if two handles (persons) are already friends
/// @param table hashtable to check
/// @param handle1 first person's handle
/// @param handle2 second person's handle
/// @return true if handle1 and handle2 ARE friends, false otherwise
bool areTheyFriendsAlr(const HashADT table, const char *handle1, const char *handle2){
    person_t* p1 = (person_t*)ht_get(table, handle1);

    // check if p1 exists
    assert(p1 != NULL);

    for (size_t i = 0; i < p1->numOfFriends; i++) {
        if(str_equals(p1 -> friends[i] -> handle, handle2)){
            return true;
        } 
    }
    return false; 
}

/// Adds a friendship between two persons identified by their handles
/// @param table hashtable to check
/// @param handle1 first person's handle
/// @param handle2 second person's handle
void addFriends(const HashADT table, const char *handle1, const char *handle2){
    person_t* p1 = (person_t*)ht_get(table, handle1);

    // check if p1 exists
    assert(p1 != NULL);

    // realloc more memory if needed
    if (p1->numOfFriends >= p1->maxFriends) {
        size_t newSize = (p1->maxFriends)*2;
        p1 -> friends = (person_t **)realloc(p1->friends, newSize * sizeof(person_t *));
        p1->maxFriends = newSize;
    }

    // Add the friend
    p1->friends[p1->numOfFriends] = (person_t*)ht_get(table, handle2);
    (p1->numOfFriends)++;

}

/// Re-initializes the system by destroying the current hash table and creating a new, empty one
/// @param table table with all information to be reinitialized
void initSystem(HashADT* table) {
    
    // Destroy the current hash table
    ht_destroy(*table);

    // Reset global counts
    numOfPplInNetwork = 0;
    numOfFriendShips = 0;

    // Create a new, empty hash table
    *table = ht_create(str_hash, str_equals, print, delete);
    printf("System re-initialized\n");
}

/// Prints information about a person, including their name, handle, and friend list
/// @param table The hash table containing the people
/// @param handle The person's handle
void printCase(const HashADT table, const char *handle) {
    person_t* person = (person_t*)ht_get(table, handle);
    
    // Check if the person exists
    if (person == NULL) {
        printf("error: handle \"%s\" is unknown\n", handle);
        return;
    }

    // Print the person's name and handle
    printf("%s (%s %s) ", person->handle, person->firstName, person->lastName);

    // Print the friend list
    if (person->numOfFriends == 0) {
        printf("has no friends\n");
    } else {
        printf("has %zu friend%s\n", person->numOfFriends, person->numOfFriends > 1 ? "s" : "");
        for (size_t i = 0; i < person->numOfFriends; i++) {
            if (person->friends[i] != NULL) {
                printf("\t%s (%s %s)\n", person->friends[i]->handle, person->friends[i]->firstName, person->friends[i]->lastName);
            }
        }
    }
}

/// Cleans up resources and exits the program
/// @param table the hashtable to be destroyed
void quitCase(HashADT table) {

    // Destroy the hash table
    ht_destroy(table);

    // Exit the program
    exit(EXIT_SUCCESS);
}

/// Prints the size of the friend list for a given person
/// @param table hashtable containing the people
/// @param handle the person's handle
void sizeCase(const HashADT table, const char *handle){
    person_t* person = (person_t*)ht_get(table, handle);

    // check if p1 exists
    assert(person != NULL);

    // Print the size of the friend list
    printf("%s (%s %s) has ", handle, person->firstName, person->lastName);
    if (person->numOfFriends == 0) {
        printf("no friends\n");
    } else if (person->numOfFriends == 1) {
        printf("1 friend\n");
    } else {
        printf("%zu friends\n", person->numOfFriends);
    }

}

/// Prints statistics about the network, including the number of people and friendships
void stats(){
    if(numOfPplInNetwork == 0){
        printf("Statistics:  no people, ");
    } else if (numOfPplInNetwork == 1){
        printf("Statistics:  1 person, ");
    } else {
        printf("Statistics:  %ld people, ", numOfPplInNetwork);
    }

    if(numOfFriendShips == 0){
        printf("no friendships\n");
    } else if (numOfFriendShips == 1){
        printf("1 friendship\n");
    } else {
        printf("%ld friendships\n", numOfFriendShips);
    }

}

/// Removes a friend from a person's friend list
/// @param person delete handle from this person's friends list 
/// @param handle handle of the friend to remove
void removeFriend(person_t* person, const char* handle) {
    for (size_t i = 0; i < person->numOfFriends; i++) {
        if (strcmp(person->friends[i]->handle, handle) == 0) {

            // Shift the remaining elements left
            for (size_t j = i; j < person->numOfFriends - 1; j++) {
                person->friends[j] = person->friends[j + 1];
            }
            person->numOfFriends--;
            break;
        }
    }
}

/// removes the friendship between 2 people
/// @param table the hashtable containing all of the peoples
/// @param handle1 handle of the first person to unfriend
/// @param handle2 handle of the second person to unfriend
void unfriend(const HashADT table, const char *handle1, const char *handle2) {
    person_t* p1 = (person_t*)ht_get(table, handle1);
    person_t* p2 = (person_t*)ht_get(table, handle2);

    // Check if both handles exist in the table
    assert(p1 != NULL && p2 != NULL);

    // Remove each friend from the other's friend list
    removeFriend(p1, handle2);
    removeFriend(p2, handle1);

}

/// frees the memory allocated for a person and their associated data
/// @param key pointer to the key associated with the person
/// @param value pointer to the value associated with the key
void delete( void *key, void *value ){
    person_t* person = (person_t*)value;
    assert(person -> friends != NULL);
    free(person -> friends);
    free(person -> firstName);
    free(person -> lastName);
    free(key);
    free(person);
}

/// prints the details of a person
/// @param key pointer to the key associated with the person
/// @param value pointer to the value associated with the key
void print( const void *key, const void *value ){
    (void)key;
    person_t* person = (person_t*) value;
    printf("%s (%s %s)\n", person->handle,person->firstName,person->lastName);
}

/// Processes commands from either standard input or a file, manipulating the social network data accordingly
/// @param isStdin boolean indicating if the input is coming from standard input
/// @param fp pointer to the file, if it exists and isStdin is false
/// @param table pointer to the hash table representing the social network
void processCommands(bool isStdin, FILE *fp, HashADT* table){
    char buffer[256];
    char *token;

    if(isStdin){
        printf("Amici> ");  // for the FIRST BEGINNING input
    }
    char *firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp)); 

    // handles " " and "\n"
    if(isStdin){
        while(firstInput == NULL){
            printf("Amici> ");
            firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp)); 
        }
    }

    while (firstInput != NULL) {
        token = strtok( buffer, DELIMS );

        if(!checkCommand(token)){
            if(isStdin){
                printf("Amici> ");
            }
            firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp));
            continue;
        }

        if(token != NULL){

            // if token and "add" are equal, strcmp returns 0 which would be, so !strcmp will be true
            if (!strcmp(token, "add")){
                // add  first-name  last-name  handle              
            
                char * firstName = strtok( NULL, DELIMS );
                char * lastName = strtok( NULL, DELIMS );
                char * handle = strtok( NULL, DELIMS );
                char * extras = strtok( NULL, DELIMS );

                if(isStdin && firstName != NULL && lastName != NULL && handle != NULL && extras == NULL){
                    printf("+ \"add\" \"%s\" \"%s\" \"%s\"\n", firstName, lastName, handle);
                }

                // checks firstName, lastName, handle is NOT NULL, checks of extras IS NULL
                // if any of these are true, it means there is too few args or too much args
                if (firstName == NULL || lastName == NULL || handle == NULL || (extras != NULL && extras[0] != '#')) {
                    printf("error: usage: add first-name last-name handle\n");
                    if(isStdin){
                        printf("Amici> "); // will be for next row inputs
                    }
                    firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp));
                    continue;
                } 

                if (checkValidName(firstName) && checkValidName(lastName) && checkHandle(handle)) {
            
                    // check if handle is alr in data base, if so print error
                    if(ht_has(*table, handle)){
                        printf("error: handle \"%s\" is already in use\n", handle);
                    } else {
                        // if handle is not in database, add to database
                        char* duppedHandle = strdup(handle);
                        char* duppedFirstName = strdup(firstName);
                        char* duppedLastName = strdup(lastName);
                        person_t * p1 = makePerson(duppedFirstName, duppedLastName, duppedHandle);
                        ht_put(*table, duppedHandle, p1);
                    }
                }

                
            } else if (!strcmp(token, "friend")){
                // friend  handle1  handle2

                char * handle1 = strtok( NULL, DELIMS );
                char * handle2 = strtok( NULL, DELIMS );
                char * extras = strtok( NULL, DELIMS );
                
                if(isStdin && handle1 != NULL && handle1 != NULL && handle2 != NULL && extras == NULL){
                    printf("+ \"friend\" \"%s\" \"%s\"\n", handle1, handle2);
                }

                // checks to make sure handle1 and handle1 are valid and that extras is NULL
                // if any of these are false, there are too few or too many args 
                if(handle1 == NULL || handle2 == NULL|| (extras != NULL && extras[0] != '#')){
                    printf("error: usage: friend handle1 handle2\n");
                    if(isStdin){
                        printf("Amici> "); // will be for next row inputs
                    }
                    firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp));
                    continue;
                } 

                // check if handle1 and handle2 are part of the database!!!!!
                // if not, print... printf("error: handle "%s" is unknown", handle1/handle2);
                
                if (checkHandle(handle1) && checkHandle(handle2) && checkIfHandleExists(*table, handle1) && checkIfHandleExists(*table, handle2) && notSamePerson(handle1, handle2)){
                    
                    // check if they're alr friends, if not make then friends
                    // otherwise print error message 
                    if (areTheyFriendsAlr(*table, handle1, handle2)){
                        printf("%s and %s are already friends.\n", handle1, handle2);
                    } else {
                        addFriends(*table, handle1, handle2);
                        addFriends(*table, handle2, handle1);

                        // update and print required output
                        numOfFriendShips++; 
                        printf("%s and %s are now friends.\n", handle1, handle2);
                        
                    }
                    
                }
                
            } else if (!strcmp(token, "init")){
                // init

                char * extras = strtok( NULL, DELIMS );

                if(extras == NULL && isStdin){
                    printf("+ \"init\"\n");
                }

                // if there are args after "init" command, its an error
                if ((extras != NULL && extras[0] != '#')){
                    printf("error: usage: init\n");
                    if(isStdin){
                        printf("Amici> "); // will be for next row inputs
                    }
                    firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp));
                    continue;
                } else {

                    // otherwise, Delete the entire contents of the system, releasing all dynamic storage
                    // Re-initialize to an empty network, print the following message, and then loop to print a new prompt
                    // message: System re-initialized
                    initSystem(table);

                    
            
                }

            } else if (!strcmp(token, "print")){
                // print handle

                char * handle = strtok( NULL, DELIMS );
                char * extras = strtok( NULL, DELIMS );

                if(isStdin && handle != NULL && extras == NULL){
                    printf("+ \"print\" \"%s\"\n", handle);
                }

                // check for valid amount of arguments
                if (handle == NULL || (extras != NULL && extras[0] != '#')){
                    printf("error: usage: print handle\n");
                    if(isStdin){
                        printf("Amici> "); // will be for next row inputs
                    }
                    firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp));
                    continue;
                }

                if (checkHandle(handle) && checkIfHandleExists(*table, handle)){
                    
                    // handle is valid
                    // Use the handle to look up the person in the table and print the person's name, handle, and the person's friend list
                    // handle (fname lname) has no friends
                    // handle (fname lname) has N friend
                    // (\t)handle (fname lname)
                    // eg: abe (A Lincoln) has 3 friends
                    //          anthony (A Blinken)
                    printCase(*table, handle);
                } 
                
            } else if (!strcmp(token, "quit")){
                // quit

                char * extras = strtok( NULL, DELIMS );

                if(isStdin && extras == NULL){
                    printf("+ \"quit\"\n");
                }

                // if there are args after "quit" command, its an error
                if ((extras != NULL && extras[0] != '#')){
                    printf("error: usage: quit\n");
                    if(isStdin){
                        printf("Amici> "); // will be for next row inputs
                    }
                    firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp));
                    continue;
                } else {
                    // otherwise, Clean up and delete all dynamic memory, and terminate with the EXIT_SUCCESS termination code
                    quitCase(*table);                  
                }
            
            } else if (!strcmp(token, "size")){     
                // size handle
                char * handle = strtok( NULL, DELIMS );
                char * extras = strtok( NULL, DELIMS );

                if(isStdin && handle != NULL && extras == NULL){
                    printf("+ \"size\" \"%s\"\n", handle);
                }
                
                // check for valid amount of arguments
                if (handle == NULL || (extras != NULL && extras[0] != '#')){
                    printf("error: usage: size handle\n");
                    if(isStdin){
                        printf("Amici> "); // will be for next row inputs
                    }
                    firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp));
                    continue;
                } else {
                    if(checkIfHandleExists(*table, handle)){
                        // otherwise...
                        // Compute and print the size of (i.e., the number of entries in) the friend list for that user, using one of the following formats according to the number of friends
                        // handle (fname lname) has no friends
                        // handle (fname lname) has 1 friend
                        // handle (fname lname) has N friends
                        sizeCase(*table, handle);
                    }
                    
                }
                
            } else if (!strcmp(token, "stats")){
                // stats
                
                char * extras = strtok( NULL, DELIMS );
                if(isStdin && extras == NULL){
                    printf("+ \"stats\"\n");
                }

                // if there are args after "quit" command, its an error
                if ((extras != NULL && extras[0] != '#')){
                    printf("error: usage: stats\n");
                    if(isStdin){
                        printf("Amici> "); // will be for next row inputs
                    }
                    firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp));
                    continue;
                } else {
                    stats();
                }

                // otherwise...
                // Print two values: the size of the network and the number of friendships
                // network size is the total number of persons in the system
                // number of friendships is the number created since the start
                // Statistics: X people, Y friendships

    
            } else if (!strcmp(token, "unfriend")){
                // unfriend  handle1  handle2

                char * handle1 = strtok( NULL, DELIMS );
                char * handle2 = strtok( NULL, DELIMS );
                char * extras = strtok( NULL, DELIMS );

                if(isStdin && handle1 != NULL && handle2 != NULL && extras == NULL){
                    printf("+ \"unfriend\" \"%s\" \"%s\"\n", handle1, handle2);
                }
                
                // checks to make sure handle1 and handle1 are valid and that extras is NULL
                // if any of these are false, there are too few or too many args 
                if(handle1 == NULL || handle2 == NULL|| (extras != NULL && extras[0] != '#')){
                    printf("error: usage: unfriend handle1 handle2\n");
                    if(isStdin){
                        printf("Amici> "); // will be for next row inputs
                    }
                    firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp));
                    continue;
                } 

                // check if handle1 and handle2 are even friends in the first place!!!!!
                // if not, print... printf("%s and %s are not friends.", handle1, handle2);

                if (checkHandle(handle1) && checkHandle(handle2) && checkIfHandleExists(*table, handle1) && checkIfHandleExists(*table, handle2) && notSamePerson(handle1, handle2)){
                    if(areTheyFriendsAlr(*table, handle1, handle2)){
                        // handle1 and handle2 are VALID, are alphanumerical n starts with letter
                        // call unfriend func
                        unfriend(*table, handle1, handle2);

                        // update and print required output
                        numOfFriendShips--; 
                        printf("%s and %s are no longer friends.\n", handle1, handle2);
                    } else {
                        printf("%s and %s are not friends.\n", handle1, handle2);
                    }
                    

                }
            } 
        } 
        if(isStdin){
            printf("Amici> ");  // will be for next row inputs AFTER it fully finishes one of the cases
        }
        firstInput = (isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp)); 
    }
}
