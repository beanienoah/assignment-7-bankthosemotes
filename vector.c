#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

// TODO - Define vector and matrix operations

// WE NEED
// -> clone, compare, add/sub, print
// very rough outline here, got everything recommended done
// rough outline was good enough :)

// TODO: CLONES
int* clone_vect(int *vector, int size) {
    int *somecopy = malloc(sizeof(int) * size);
    if(!somecopy) {
        return NULL;
    }
    for(int i = 0; i < size; i++) {
        somecopy[i] = vector[i];
    }
    return somecopy;
}

int** clone_neo(int **matrix, int rows, int columns) {
    int **somecopy = malloc(sizeof(int*) * rows);
    if(!somecopy) {
        return NULL;
    }
    for(int i = 0; i < rows; i++) {
        somecopy[i] = clone_vect(matrix[i], columns);
        if(!somecopy[i]) {
            return NULL;
        }
    }
    return somecopy;
}

// TODO: COMAPRE (< (and) ==)
int compare1(int *x, int *y, int size) {
    for(int i = 0; i < size; i++) {
        if(x[i] > y[i]) {
            return 0; // everything but this!
        }
    }
    return 1; // less than or equal for all i
}

int compare2(int *x, int *y, int size) {
    for(int i = 0; i < size; i++) {
        if(x[i] != y[i]) { 
            return 0;
        }
    }
    return 1; // equal for all i
}

// TODO: OPERATIONS
void add_vect(int *addto, int *this, int size) {
    for(int i = 0; i < size; i++) {
        addto[i] += this[i];
    }
}

void sub_vect(int *subfrom, int *this, int size) {
    for(int i = 0; i < size; i++) {
        subfrom[i] -= this[i];
    }
}

// TODO: PRINT (what the hell do i need these for? lol)
// ate those words this was somewhat good for debugging
void print_vect(int *vector, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d", vector[i]);
    }
    printf("\n");
}

void print_neo(int **matrix, int rows, int columns) {
    for(int i = 0; i < rows; i++) {
        print_vect(matrix[i], columns);
    }
}

