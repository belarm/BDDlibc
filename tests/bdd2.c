#include "bdd2.h"


#define HASH_SIZE 24593
#define _get_truth(X) (X.hi == 1)
#define _is_sink(X) ((X).v == (X).bdd->terms)

struct bdd_node {
	int v;
	bdd_node *hi, *lo;
	int aux;
	bdd *base;
};

struct bdd_hash_entry {
	bdd_node *node;
	bdd_hash_entry *next;
};

struct bdd_hash_table {
	int count;
	bdd_hash_entry *table[HASH_SIZE];
};

struct bdd {
	int count;
	int terms;
	int capacity;
	bdd_node *I;
	bdd_hash_table unique;
};

void new_bdd(bdd **target, int terms);
