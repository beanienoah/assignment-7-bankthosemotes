/* declarations related to banker's algorithm */

int sanity_check(int *totalres, int **alloc_neo, int**max_neo, int NRES, int NPROC);
int is_safe(int *Available, int **Alloc, int **Need, int NRES, int NPROC);