/// @file processArgs
/// @author Kunlin Wen, kw9521

#define DELIMITERS   " \t"        // separate tokens using whitespace
#define DELIMSWITHNEWLINE " \t\n"
#define _DEFAULT_SOURCE             // must be before all #include to be able to use getline()

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
#include "processArgs.h"

size_t numOfPplInNetwork = 0;
size_t numOfFriendShips = 0;

typedef struct person_s {
    char *firstName;            // name of the person
    char *lastName;
    char *handle;               // handle of the person
    struct person_s **friends;  // dynamic collection of friends
    size_t numOfFriends;        // number of friendships 
    size_t maxFriends;    // current limit on friends, need if ur using dynamic array that doubles in size everytme the array is full
} person_s;

person_t makePerson(char* firstName, char* lastName, char* handle){
    person_s * person = (person_s*)malloc(sizeof(person_s));

    // check if memeory was correctly allocated
    assert(person != NULL && firstName != NULL && lastName !=NULL && handle !=NULL);

    person -> maxFriends = 100;
    person -> firstName = firstName;
    person -> lastName = lastName;
    person -> handle = handle;
    person-> friends = (person_s**)malloc(person->maxFriends * sizeof(person_s*));
    assert(person-> friends != NULL);
    person -> numOfFriends = 0;

    // increase the num of people made 
    printf("firstname: %s, lastname: %s, handle: %s\n", firstName, lastName, handle);
    numOfPplInNetwork++;
    return person;
}

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
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != '\'' && name[i] != '-') {
            printf("error: argument \"%s\" is invalid\n", name);
            return false;
        }
    }

    return true;
}

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
    for (int i = 0; handle[i] != '\0'; i++) {
        if (!isalnum(handle[i])) {
            printf("error: argument \"%s\" is invalid\n", handle);
            return false;
        }
    }

    return true;
}

void checkHandlesInTble(const HashADT table){
    void **keys = ht_keys(table);

        printf("Current handles in the table:\n");
        for (size_t i = 0; keys[i] != NULL; i++) {
            printf("%s, ", (char *)keys[i]);
        }

        // Free the dynamically allocated array of keys
        free(keys);
}

bool checkIfHandleExists(const HashADT table, const char *handle){

    // for testing:
    checkHandlesInTble(table);

    if (ht_has(table, handle)) {
        return true;
    }
    printf("error: handle \"%s\" is unknown\n", handle);
    return false; 
    
}

bool areTheyFriendsAlr(const HashADT table, const char *handle1, const char *handle2){
    person_s* p1 = (person_s*)ht_get(table,handle1);

    // check if p1 exists
    assert(p1 != NULL);

    for (size_t i = 0; i < p1->numOfFriends; i++) {
        if(p1->friends[i] != NULL && strcmp(p1->friends[i]->handle, handle2) == 0){
            return true;
        } 
    } 
    return false; 
}

void addFriends(const HashADT table, const char *handle1, const char *handle2){
    person_s* p1 = (person_s*)ht_get(table, handle1);
    person_s* p2 = (person_s*)ht_get(table, handle2);

    // check if p1 and p2 exists
    assert(p1 != NULL && p2 != NULL);

    if (p1->numOfFriends >= p1->maxFriends) {
        size_t newSize = p1->maxFriends * 2;
        p1->friends = (person_s **)realloc(p1->friends, newSize * sizeof(person_s *));
        
        assert(p1->friends != NULL);
        p1->maxFriends = newSize;
    }
    
    // Add the friend
    p1->friends[p1->numOfFriends] = p2;
    p1->numOfFriends++;

}

void initSystem(HashADT table) {
    // Get all values (persons) in the hash table
    void **allPersons = ht_values(table);
    if (allPersons != NULL) {
        for (size_t i = 0; allPersons[i] != NULL; i++) {
            person_s *person = (person_s *)allPersons[i];
            // Free the friends array
            free(person->friends);
            // Free the person struct itself
            free(person -> handle);
            free(person -> firstName);
            free(person -> lastName);
            free(person);
        }
        // Free the array holding all persons
        free(allPersons);
    }

    // Destroy the current hash table
    ht_destroy(table);

    // Reset global counts
    numOfPplInNetwork = 0;
    numOfFriendShips = 0;

    // Create a new, empty hash table
    table = ht_create(str_hash, str_equals, print, delete);
    printf("System re-initialized\n");
}


void printCase(const HashADT table, const char *handle) {
    person_s *person = (person_s *)ht_get(table, handle);
    
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

void quitCase(HashADT table) {
    // Free all persons
    void **allPersons = ht_values(table);
    if (allPersons != NULL) {
        for (size_t i = 0; allPersons[i] != NULL; i++) {
            person_s *person = (person_s *)allPersons[i];
            free(person->friends);
            free(person->handle);
            free(person->firstName);
            free(person->lastName);
            free(person);
        }
        free(allPersons);
    }

    // Destroy the hash table
    ht_destroy(table);

    // If there are other resources to free, do it here

    // Exit the program
    exit(EXIT_SUCCESS);
}


void sizeCase(const HashADT table, const char *handle){
    person_s* person = (person_s*)ht_get(table, handle);

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
        printf("1 friendships\n");
    } else {
        printf("%ld friendships\n", numOfFriendShips);
    }

}

void removeFriend(person_t person, const char* handle) {
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

void unfriend(const HashADT table, const char *handle1, const char *handle2) {
    person_s* p1 = (person_s*)ht_get(table, handle1);
    person_s* p2 = (person_s*)ht_get(table, handle2);

    // Check if both handles exist in the table
    assert(p1 != NULL && p2 != NULL);

    // Remove each friend from the other's friend list
    removeFriend(p1, handle2);
    removeFriend(p2, handle1);

    numOfFriendShips -= 1;
}

void delete( void *key, void *value ){
    (void)key;
    person_s * person = (person_t) value;
    assert(person -> friends != NULL);
    free(person->friends);
    free(person);
}

void print( const void *key, const void *value ){
    (void)key;
    person_s* person = (person_t) value;
    printf("%s (%s %s)\n", person->handle,person->firstName,person->lastName);
}


void processCommands(bool isStdin, FILE *fp, HashADT* table){
    char buffer[256];
    while ((isStdin ? fgets(buffer, 256, stdin) : fgets(buffer, 256, fp)) != NULL) {
        char *token;

        // first call specifies the buffer to be parsed
        // gets rid of space, tabs and new lines
        token = strtok( buffer, DELIMSWITHNEWLINE );   
    

        if(isStdin){
            printf("Amici> ");
        }
        while (token != NULL) {
            
            // if token and "add" are equal, strcmp returns 0, if, so !strcmp will be true
            if (!strcmp(token, "add")){
                // add  first-name  last-name  handle              
            
                char * firstName = strtok( NULL, DELIMITERS );
                char * lastName = strtok( NULL, DELIMITERS );
                char * handle = strtok( NULL, DELIMSWITHNEWLINE );
                char * extras = strtok( NULL, DELIMITERS );

                if(firstName != NULL && lastName != NULL && handle != NULL && extras == NULL){
                    printf("+ \"add\" \"%s\" \"%s\" \"%s\"\n", firstName, lastName, handle);
                }

                // checks firstName, lastName, handle is NOT NULL, checks of extras IS NULL
                // if any of these are true, it means there is too few args or too much args
                if (firstName == NULL || lastName == NULL || handle == NULL || (extras != NULL && extras[0] != '#')) {
                    printf("error: usage: add first-name last-name handle\n");
                } 

                if (checkValidName(firstName) && checkValidName(lastName) && checkHandle(handle)) {
                    

                    // HT_HAS IS NOT WORKING 


                    // check if handle is alr in data base, if so print error
                    if(ht_has(*table, handle)){
                        printf("error: handle \"%s\" is already in use\n", handle);
                    } else {
                        // if handle is not in database, add to database
                        person_s * p1 = makePerson(firstName, lastName, handle);
                        ht_put(*table, handle, p1);
                    }
                }

                
            } else if (!strcmp(token, "friend")){
                // friend  handle1  handle2

                char * handle1 = strtok( NULL, DELIMITERS );
                char * handle2 = strtok( NULL, DELIMSWITHNEWLINE );
                char * extras = strtok( NULL, DELIMITERS );
                
                if(handle1 != NULL && handle1 != NULL && handle2 != NULL && extras == NULL){
                    printf("+ \"friend\" \"%s\" \"%s\"\n", handle1, handle2);
                }

                // checks to make sure handle1 and handle1 are valid and that extras is NULL
                // if any of these are false, there are too few or too many args 
                if(handle1 == NULL || handle2 == NULL|| (extras != NULL && extras[0] != '#')){
                    printf("error: usage: friend handle1 handle2\n");
                } 

                // check if handle1 and handle2 are part of the database!!!!!
                // if not, print... printf("error: handle "%s" is unknown", handle1/handle2);
                
                if (checkHandle(handle1) && checkHandle(handle2) && checkIfHandleExists(*table, handle1) && checkIfHandleExists(*table, handle2)){
                    
                    // check if they're alr friends, if not make then friends
                    // otherwise print error message 
                    if (areTheyFriendsAlr(*table, handle1, handle2)){
                        printf("%s and %s are already friends\n", handle1, handle2);
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

                char * extras = strtok( NULL, DELIMITERS );

                if(extras == NULL){
                    printf("+ \"init\"\n");
                }

                // if there are args after "init" command, its an error
                if ((extras != NULL && extras[0] != '#')){
                    printf("error: usage: init\n");
                } else {

                    // otherwise, Delete the entire contents of the system, releasing all dynamic storage
                    // Re-initialize to an empty network, print the following message, and then loop to print a new prompt
                    // message: System re-initialized

                    initSystem(*table);
            
                }

            } else if (!strcmp(token, "print")){
                // print handle

                char * handle = strtok( NULL, DELIMSWITHNEWLINE );
                char * extras = strtok( NULL, DELIMITERS );

                if(handle != NULL && extras == NULL){
                    printf("+ \"print\" \"%s\"\n", handle);
                }

                // check for valid amount of arguments
                if (handle == NULL || (extras != NULL && extras[0] != '#')){
                    printf("error: usage: print handle\n");
                }

                if (checkHandle(handle)){
                    
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

                char * extras = strtok( NULL, DELIMITERS );

                if(extras == NULL){
                    printf("+ \"quit\"\n");
                }

                // if there are args after "quit" command, its an error
                if ((extras != NULL && extras[0] != '#')){
                    printf("error: usage: quit\n");
                } else {
                    // otherwise, Clean up and delete all dynamic memory, and terminate with the EXIT_SUCCESS termination code
                    quitCase(*table);                  
                }


            
            } else if (!strcmp(token, "size")){     // SEG FAULT HEREEEEE
                // size handle
                char * handle = strtok( NULL, DELIMSWITHNEWLINE );
                char * extras = strtok( NULL, DELIMITERS );

                if(handle != NULL && extras == NULL){
                    printf("+ \"size\" \"%s\"\n", handle);
                }
                
                // check for valid amount of arguments
                if (handle == NULL || (extras != NULL && extras[0] != '#')){
                    printf("error: usage: size handle\n");
                } else {
                    // otherwise...
                    // Compute and print the size of (i.e., the number of entries in) the friend list for that user, using one of the following formats according to the number of friends
                    // handle (fname lname) has no friends
                    // handle (fname lname) has 1 friend
                    // handle (fname lname) has N friends
                    sizeCase(*table, handle);
                }
                
            } else if (!strcmp(token, "stats")){
                // stats
                
                char * extras = strtok( NULL, DELIMITERS );
                if(extras == NULL){
                    printf("+ \"stats\"\n");
                }

                // if there are args after "quit" command, its an error
                if ((extras != NULL && extras[0] != '#')){
                    printf("error: usage: stats\n");
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

                char * handle1 = strtok( NULL, DELIMITERS );
                char * handle2 = strtok( NULL, DELIMSWITHNEWLINE );
                char * extras = strtok( NULL, DELIMITERS );

                if(handle1 != NULL && handle2 != NULL && extras == NULL){
                    printf("+ \"unfriend\" \"%s\" \"%s\"\n", handle1, handle2);
                }
                
                // checks to make sure handle1 and handle1 are valid and that extras is NULL
                // if any of these are false, there are too few or too many args 
                if(handle1 == NULL || handle2 == NULL|| (extras != NULL && extras[0] != '#')){
                    printf("error: usage: unfriend handle1 handle2\n");
                } 

                // check if handle1 and handle2 are even friends in the first place!!!!!
                // if not, print... printf("%s and %s are not friends.", handle1, handle2);

                if (checkHandle(handle1) && checkHandle(handle2)){
                    
                    // handle1 and handle2 are VALID, are alphanumerical n starts with letter
                    // call unfriend func
                    unfriend(*table, handle1, handle2);

                    // update and print required output
                    numOfFriendShips--; 
                    printf("%s and %s no longer friends.\n", handle1, handle2);

                }
                
            } else if (strcmp(token, "\0")) {        
                // command does not meet this: which consist solely of lowercase alphabetic characters. Commands appear only as the first token on an input line
                break;;
            } else {
                continue;
            }
            // subsequent calls to parse the same buffer
            token = strtok( NULL, DELIMSWITHNEWLINE );
        }
    }

}