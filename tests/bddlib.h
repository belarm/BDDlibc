#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef _BDD_HEADER
#define _BDD_HEADER

struct BDD;
typedef struct BDD BDD;

int get_bdd_count(BDD *bdd);
void add_to_bdd(BDD *bdd, int v, int lo, int hi);
BDD *new_bdd(BDD **bdd, int terms);
int get_bdd_count(BDD *bdd);
int print_bdd(BDD *bdd);


#endif /* _BDD_HEADER */