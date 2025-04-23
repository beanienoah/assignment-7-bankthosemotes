#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

// TODO - Safety Algorithm goes here

int sanity_check(int *totalres, int **alloc_neo, int **max_neo, int NRES, int NPROC) {
    // check that allocated resources do not exceed total num resources
    int *alloc_check = malloc(NRES * sizeof(int));
    for(int i = 0; i < NPROC; i++) {
        for(int j = 0; j < NRES; j++) {
            alloc_check[j] += alloc_neo[i][j]; // allocations totaled
        }
    }
    
    for(int i = 0; i < NRES; i++) {
        if (alloc_check[i] > totalres[i]) {
            printf("Integrity test failed: allocated resources exceed total resources\n");
            free(alloc_check);
            return 1;
        }
    }

    // check that allocations dont exceed max demand
    for(int i = 0; i < NPROC; i++) {
        for(int j = 0; j < NRES; j++) {
            if(alloc_neo[i][j] > max_neo [i][j]) {
                int error = max_neo[i][j] - alloc_neo[i][j];
                printf("Integrity test failed: process allocation exceed demand for Thread %d\n", i);
                printf("Need %d instances of resource %d\n", error, j);
                free(alloc_check);
                return 1;

            }
        }
    }

    free(alloc_check);
    return 0;
}


// Finds all safe orderings. Kept outside of main is_safe func because it's a mess.
void find_safe(int *work, int **Alloc, int **Need, int *finish, int *path, int depth, int NRES, int NPROC) {
    int finished = 1; // ASSUME all threads are finished
    for(int i = 0; i < NPROC; i++) {
        if(!finish[i]) {
            finished = 0; // 
            if(compare1(Need[i], work, NRES)) {
                // fake run (can i finish? <= work?)
                finish[i] = 1;
                path[depth] = i;
                add_vect(work, Alloc[i], NRES); // release
                // recurse
                find_safe(work, Alloc, Need, finish, path, depth+1, NRES, NPROC);
                // backtrack
                sub_vect(work, Alloc[i], NRES);
                finish[i] = 0;
            }
        }
    }
    if(finished) {
        // print out schedule
        printf("SAFE: ");
        for(int i = 0; i < NPROC; i++) {
            printf("T%d ", path[i]);
        }
        printf("\n");
    }
}

/**
* Approximation of your pseudo code, with NRES and NPROC built in.
* Tried to recurse inside this, ended up refactoring into another method.
* Okay. That worked for safe but now unsafe is acting funky.
* OKAY 
* nevermind
* just rerefactored what i already knew worked, so stupid and inefficient
* but whatever it works. smarter idea would've been to have had an issafe
* and an isnotsafe and keep their resources separate. detects unsafe
* states (if there are any), or prints all valid safe schedules
* (pseudocode is unrecognizeable lol)
* 
*/
int is_safe(int *Available, int **Alloc, int **Need, int NRES, int NPROC) {
    // fake check
    int *unsafe_work = clone_vect(Available, NRES); // clone available -> work
    int *unsafe_check = malloc(NPROC * sizeof(int));
    int check_fake = 1;
    // run as many as possible
    while(check_fake) {
        check_fake = 0;
        for(int i = 0; i < NPROC; i++) {
            if(!unsafe_check[i] && compare1(Need[i], unsafe_work, NRES)) {
                add_vect(unsafe_work, Alloc[i], NRES);
                unsafe_check[i] = 1;
                check_fake = 1;
            }
        }
    }
    // if any are unfinished then we know this is unsafe
    int check_sum = 1;
    for(int i = 0; i < NPROC; i++) {
        if(!unsafe_check[i]) {
            check_sum = 0;
            break;
        }
    }

    if(!check_sum) {
        // print out unsafe
        printf("UNSAFE: ");
        for(int i = 0; i < NPROC; i++) {
            if(!unsafe_check[i]) {
                printf("T%d ", i);
            }
        }
        printf("can't finish\n");
        free(unsafe_work);
        free(unsafe_check);
        return 0;
    }
    
    // now it must be safe, print out all possible safe schedules
    int *work = clone_vect(Available, NRES); // clone available -> work
    int *finish = malloc(NPROC * sizeof(int)); // for all 0 <= i < n (NPROC) -> finish
    int *path = malloc(NPROC * sizeof(int));
    // find safe
    find_safe(work, Alloc, Need, finish, path, 0, NRES, NPROC);
    // clean up
    free(work);
    free(finish);
    free(path);
    return (1);
}