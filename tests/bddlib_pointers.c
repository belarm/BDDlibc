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


struct bddp_hash_entry_s {
	struct bdd_node_pointers_s *node;
	struct bddp_hash_entry_s *next;
};

struct bddp_hash_table_s {
	int count;
	struct bddp_hash_entry_s *table[HASH_SIZE];
};

struct BDDP {
	int count;
	int terms;
	int capacity;
	struct bddp_hash_table_s unique;
	struct bdd_node_pointers_s *I;
};

unsigned long bddb_hash(bddp_node node) {
	int ret = 1;
	ret = ret * 31 + node.v;
	ret = ret * 31 + (unsigned long)node.hi;
	return (ret * 31 + (unsigned long)node.lo) % HASH_SIZE;
}

int add_to_hashtable_p(bddp_node *node) {
	uint hash_val = bddp_hash(*node);
	hash_node *entry = malloc(sizeof(hash_node));
	entry->node = node;
	entry->next = NULL;
	if(!node->bdd->unique.table[hash_val]) {
		node->bdd->unique.table[hash_val] = entry;
		node->bdd->unique.count++;
		return 1;
	} else {
		hash_node *in_table = node->bdd->unique.table[hash_val];
		while(true) {
			if(nodes_equal(*in_table->node,*node)) {
				return -1;
			}
			if(in_table->next == NULL)
				break;
			in_table = in_table->next;
		}
		printf("No dupe found, adding to bucket\n");
		in_table->next = entry;
		node->bdd->unique.count++;
		return 1;
	}
}
