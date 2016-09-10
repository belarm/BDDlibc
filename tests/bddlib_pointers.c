#include "bddlib_pointers.h"

#define HASH_SIZE 24593
#define _get_truth(X) (X.hi == 1)
#define _is_sink(X) ((X).v == (X).bdd->terms)





/*
Operators needed: == < > ...

Actually, implement each BOOL_OP as an operator with a short name for clear boolean algebra.
Operators can simply call bdd_op with the relevant value.

Also need operators for ordering and sorting, so comparison, equality/equivalence...

*/

struct bdd_node_pointers_s {
	int v;
	struct bdd_node_pointer_s *hi, *lo;
	int aux, idx;
	BDD *base;
};


struct bddp_hash_entry {
	struct bdd_node_pointers_s *node;
	struct bddp_hash_entry *next;
};

struct bddp_hash_table {
	int count;
	struct bddp_hash_entry *table[HASH_SIZE];
};

struct BDDP {
	int count;
	int terms;
	int capacity;
	struct bddp_hash_table unique;
	struct bdd_node_pointers_s *I;
};

