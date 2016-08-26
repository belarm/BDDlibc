#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "bddlib.h"

#define HASH_SIZE 24593
#define _get_truth(X) (X.hi == 1)
#define _is_sink(X) ((X).v == (X).bdd->terms)



struct bdd_node_s {
	uint v;
	uint lo;
	uint hi;
	BDD *bdd; //can nodes be free?
};

struct hash_node_s {
	bdd_node *node;
	struct hash_node_s *next;
};

struct hash_table_s {
	int count;
	hash_node *table[HASH_SIZE];
};

struct BDD {
	int count;
	int terms;
	int capacity;
	struct hash_table_s unique;
	struct bdd_node_s *I;
};



uint bdd_hash(bdd_node node) {
	int ret = 1;
	ret = ret * 31 + node.v;
	ret = ret * 31 + node.hi;
	return (ret * 31 + node.lo) % HASH_SIZE;
}

int add_to_hashtable(bdd_node *node) {
	//h_table->count++;
	//hash_table *h_table = bdd->*unique;
	uint hash_val = bdd_hash(*node);
	//printf("Got hash value %d\n",hash_val);
	hash_node *entry = malloc(sizeof(hash_node));
	entry->node = node;
	entry->next = NULL;
	if(!node->bdd->unique.table[hash_val]) {
		node->bdd->unique.table[hash_val] = entry;
		node->bdd->unique.count++;
		return 1;
	} else {
		//printf("Collision!\n");
		hash_node *in_table = node->bdd->unique.table[hash_val];
		while(true) {
			if(nodes_equal(*in_table->node,*node)) {
				//printf("Found duplicate in bucket\n");
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

bdd_node _add_to_bdd(BDD *bdd, bdd_node node) {
	return bdd->I[add_to_bdd(bdd, node.v, node.lo, node.hi)];
}

int add_to_bdd(BDD *bdd, int v, int lo, int hi) {
	
	bdd_node temp = {.v = v, .lo = lo, .hi = hi, .bdd = bdd};
	//int hash = bdd_hash(temp);
	add_to_hashtable(&temp);
	bdd->I[bdd->count] = temp;
	return bdd->count++;
}

BDD *new_bdd(BDD **in, int terms ) {
	BDD *ret = (BDD *)malloc(sizeof(BDD));
	ret->terms = terms;
	ret->capacity = 1024;
	ret->I = malloc(sizeof(bdd_node) * 1024);
	ret->count = 0;
	add_to_bdd(ret,terms,0,0); //False sink
	add_to_bdd(ret,terms,1,1); //True sink
	*in = ret;
	return *in;
}

hash_node *get_new_hashtable() {
	return (hash_node *)malloc(sizeof(hash_node) * HASH_SIZE);
}

bool nodes_equal(bdd_node a, bdd_node b) {
	return (a.v == b.v && a.hi == b.hi && a.lo == b.lo);
}

int get_bdd_count(BDD *bdd){
	printf("Got BDD %p\n",bdd);
	return bdd->count;
}

int print_bdd(BDD *bdd) {
	int i;
	for(i=0;i<bdd->count;i++) {
		printf("%d\t%d?%d:%d\n",i,bdd->I[i].v,bdd->I[i].lo,bdd->I[i].hi);
	}
	return i;
}

int make_dot(BDD *bdd, char *path) {
	if(strlen(path) == 0)
		return 0;
	FILE *out = fopen(path,"w");
	fprintf(out,"digraph bdd {\n");
	fprintf(out,"\tnode [shape=record];\n");
	for(int i = 0;i<bdd->count;i++) {
		fprintf(out,"\tnode%d [label = \"{ %d | { <v> %d|<lo> %d|<hi> %d } }\"];\n",i,i,bdd->I[i].v,bdd->I[i].lo,bdd->I[i].hi);
	}
	for(int i = 2;i<bdd->count;i++) {
		fprintf(out,"\tnode%d:hi -> node%d [color=blue];\n",i,bdd->I[i].hi);
		fprintf(out,"\tnode%d:lo -> node%d [style=dashed color=red];\n",i,bdd->I[i].lo);
	}
	fprintf(out,"}\n");
	fclose(out);
	return bdd->count;
}

bdd_node get_node(BDD *bdd, int node) {
	return bdd->I[node];
}

bdd_node get_hi(bdd_node node) {
	return node.bdd->I[node.hi];
}

bdd_node get_lo(bdd_node node) {
	return node.bdd->I[node.lo];
}

union packed_op {
	struct {
		unsigned int word1 : 4;
		unsigned int word2 : 4;
		unsigned int word3 : 4;
		unsigned int word4 : 4;
		unsigned int word5 : 4;
		unsigned int word6 : 4;
		unsigned int word7 : 4;
		unsigned int word8 : 4;
	};
	unsigned int all;
};

bool bdd_op(enum BOOL_OP op, bool a, bool b){
	switch (op) {
		case FALSE:				//0000
			return false;
		case AND:				//0001
			return a && b;
		case A_AND_NOT_B:		//0010
			return a && !b;
		case A:					//0011
			return a;
		case NOT_A_AND_B:		//0100
			return !a && b;
		case B:					//0101
			return b;
		case XOR:				//0110
			return a ^ b;
		case OR:				//0111
			return a | b;
		case NOR:				//1000
			return !(a|b);
		case XNOR:				//1001
			return !(a^b);
		case NOT_B:				//1010
			return !b;
		case A_OR_NOT_B:		//1011
			return a || !b;
		case NOT_A:				//1100
			return !a;
		case NOT_A_OR_B:		//1101
			return !a || b;
		case NAND:				//1110
			return !(a && b);
		default:				//1111
			return true;
	}
}

int _BDD_A (BDD **res, BDD *a, BDD *b, int node_a, int node_b) {
	bdd_node na = get_node(a,node_a);
	bdd_node nb = get_node(b,node_b);
	//Here's where we get recursive, baby!
	if(na.v == na.v) {
		return add_to_bdd(
			*res,								//BDD
			na.v,								// .v
			_BDD_A(res,a,b,na.lo,nb.lo),		//.lo
			_BDD_A(res,a,b,na.hi,nb.hi)			//.hi
		);
	} else if (na.v < nb.v) {
		return add_to_bdd(
			*res,								//BDD
			na.v,								// .v
			_BDD_A(res,a,b,na.lo,node_b),		//.lo
			_BDD_A(res,a,b,na.hi,node_b)		//.hi
		);
	} else { //na.v > nb.v
			return add_to_bdd(
			*res,								//BDD
			nb.v,								// .v
			_BDD_A(res,a,b,node_a,nb.lo),		//.lo
			_BDD_A(res,a,b,node_a,nb.hi)		//.hi
		);
	}
}

int _meld (BDD *target, enum BOOL_OP op, bdd_node f, bdd_node g) {
	if(_is_sink(f) && _is_sink(g)) {
		return bdd_op(op, _get_truth(f), _get_truth(g));
	}
	if(f.v == g.v) {
		return add_to_bdd(
			target,
			f.v,
			_meld(
				target,
				op,
				get_lo(f),
				get_lo(g)
			),
			_meld(
				target,
				op,
				get_hi(f),
				get_hi(g)
			)
		);
	} else if (f.v < g.v ){
		return add_to_bdd(
			target,
			f.v,
			_meld(
				target,
				op,
				get_lo(f),
				g
			),
			_meld(
				target,
				op,
				get_hi(f),
				g
			)
		);
	} else {
		return add_to_bdd(
			target,
			g.v,
			_meld(
				target,
				op,
				f,
				get_lo(g)
			),
			_meld(
				target,
				op,
				f,
				get_hi(g)
			)
		);
	}
}

int meld(BDD *target, enum BOOL_OP op, BDD *f, BDD *g) {
	return _meld(target,op,get_node(f,f->count-1),get_node(g,g->count-1));
}