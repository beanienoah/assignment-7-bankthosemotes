#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

int main(int argc, char *argv[])
{
  // TODO: attempt to open scenario file and scan data into allocated structures
  if(argc < 2) {
    printf("No input file provided!\n");
    return 1;
  }

  FILE *fPile = fopen(argv[1], "r"); // "pFile" sounds so bad LMAO
  if(!fPile) {
    printf("File not found!\n");
    return 1;
  }

  // TODO: setup and read data
  int NRES; // resources types
  int NPROC; // processes
  fscanf(fPile, "%d", &NRES);
  fscanf(fPile, "%d", &NPROC);

  // total resources
  int *totalres = malloc(NRES * sizeof(int));
  for(int i = 0; i < NRES; i++) {
    fscanf(fPile, "%d", &totalres[i]);
  }

  // max matrix (first)
  int **max_neo = malloc(NPROC * sizeof(int*));
  for(int i = 0; i < NPROC; i++) {
    max_neo[i] = malloc(NRES * sizeof(int));
    for(int j = 0; j < NRES; j++) {
      fscanf(fPile, "%d", &max_neo[i][j]);
    }
  }

  // alloc matrix (last)
  int **alloc_neo = malloc(NPROC * sizeof(int*));
  for(int i = 0; i< NPROC; i++) {
    alloc_neo[i] = malloc(NRES * sizeof(int));
    for(int j = 0; j < NRES; j++) {
      fscanf(fPile, "%d", &alloc_neo[i][j]);
    }
  }

  // sanity check! threw this in bankers because it was clogging up main
  if(sanity_check(totalres, alloc_neo, max_neo, NRES, NPROC)) {
    return 1; // bad sanity!
  }

  // TODO: Run banker's safety algorithm 
  // isSafe(Available, Alloc, Need)
  // find Available
  int *Available = clone_vect(totalres, NRES); // initial is total
  for(int i = 0; i < NPROC; i++) {
    sub_vect(Available, alloc_neo[i], NRES); // -allocation
  }

  // find Need (max - alloc)
  int **Need = malloc(NPROC * sizeof(int*));
  for(int i = 0; i < NPROC; i++) {
    Need[i] = malloc(NRES * sizeof(int));
    for(int j = 0; j < NRES; j++) {
      Need[i][j] = max_neo[i][j] - alloc_neo[i][j];
    }
  }

  // run bankers safety check
  is_safe(Available, alloc_neo, Need, NRES, NPROC);
    //printf("SUCCESS!\n"); YESSSSS
    // FUCK

  // TODO: cleanup
  free(totalres);
  for(int i = 0; i < NPROC; i++) {
    free(max_neo[i]);
    free(alloc_neo[i]);
    free(Need[i]);
  }
  free(max_neo);
  free(alloc_neo);
  free(Need);
  free(Available);
  fclose(fPile);
  return 0;
}