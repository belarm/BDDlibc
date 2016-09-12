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

bool _nodes_equal(bdd_node a, bdd_node b) {
	return (a.v == b.v && a.hi == b.hi && a.lo == b.lo);
}

unsigned int _get_hash(bdd_node node) {
	int ret = 1;
	ret = ret * 31 + node.v;
	ret = ret * 31 + (unsigned long)node.hi;
	return (ret * 31 + (unsigned long)node.lo) % HASH_SIZE;
}

int _unique_insert(bdd_node *node) {
	unsigned int hash_val = _get_hash(*node);
	bdd_hash_entry *entry = malloc(sizeof(bdd_hash_entry));
	entry->node = node;
	entry->next = NULL;
	if(!node->base->unique.table[hash_val]) {
		node->base->unique.table[hash_val] = entry;
		node->base->unique.count++;
		return 1;
	} else {
		bdd_hash_entry *in_table = node->base->unique.table[hash_val];
		while(true) {
			if(_nodes_equal(*in_table->node,*node)) {
				return -1;
			}
			if(in_table->next == NULL)
				break;
			in_table = in_table->next;
		}
		printf("No dupe found, adding to bucket\n");
		in_table->next = entry;
		node->base->unique.count++;
		return 1;
	}
}

bdd_hash_entry *get_new_hashtable() {
	return (bdd_hash_entry *)malloc(sizeof(bdd_hash_entry) * HASH_SIZE);
}

int add_node_to_bdd(bdd *base, int v, bdd_node *lo, bdd_node *hi) {
	bdd_node temp = {.v = v, .lo = lo, .hi = hi, .base = base, .aux = 0};
	_unique_insert(&temp);
	base->I[base->count] = temp;
	return base->count++;
}

bdd_node _add_to_bdd(bdd *base, bdd_node node) {
	return base->I[add_node_to_bdd(base, node.v, node.lo, node.hi)];
}


int add_to_bdd(bdd *base, int v, int lo_idx, int hi_idx) {
	bdd_node *lo = (bdd_node *)(base->I + lo_idx * sizeof(bdd_node));
	bdd_node *hi = (bdd_node*)(base->I + hi_idx * sizeof(bdd_node));
	
	bdd_node temp = {.v = v, .lo = lo, .hi = hi, .base = base, .aux = 0};
	_unique_insert(&temp);
	base->I[base->count] = temp;
	return base->count++;
}

void new_bdd(bdd **in, int terms)  {
	bdd *ret = malloc(sizeof(bdd));
	ret->terms = terms;
	ret->capacity = 1024;
	ret->I = malloc(sizeof(bdd_node) * ret->capacity);
	ret->count = 0;
	add_to_bdd(ret,terms,0,0);
	add_to_bdd(ret,terms,1,1);
	*in = ret;
}

