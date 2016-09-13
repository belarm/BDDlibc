
#include "bdd2.h"


#define HASH_SIZE 24593
#define _get_truth(X) ((X)->hi == (X)->base->I + sizeof(bdd_node)) //does ->hi point to T?
#define _is_sink(X) ((X)->v == (X)->base->terms)

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


//~~~~~~~~~~~~~~~~~~~~~~~~~~
//TODO:
//Replace with 16 pointers to inline functions and check assembler output
//...just to see what happens.
//inlining will likely not work in most cases, but the function stacks may be interesting
//Also: Generate executable from BDD and static functions?
//~~~~~~~~~~~~~~~~~~~~~~~~~~

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

int get_bdd_count(bdd *base) {
	return base->count;
}

bdd_node get_node(bdd *base, int idx) {
	return base->I[idx];
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

int print_bdd(bdd *base) {
	int i;
	for(i=0;i<base->count;i++) {
		printf("%d\t%d?%ld:%ld\n",i,base->I[i].v,(base->I[i].lo-base->I)/sizeof(bdd_node),(base->I[i].hi-base->I)/sizeof(bdd_node));
	}
	return i;
}

int _meld (bdd *target, enum BOOL_OP op, bdd_node *f, bdd_node *g) {
	printf("Checking if 1:%ld or 2:%ld are sinks\n",(f-f->base->I)/sizeof(bdd_node),(g-g->base->I)/sizeof(bdd_node));
	fflush(stdout);
	if(_is_sink(f) && _is_sink(g)) {
		return bdd_op(op, _get_truth(f), _get_truth(g));
	}
	if(f->v == g->v) {
		return add_to_bdd(target,f->v,_meld(target,op,f->lo,g->lo),_meld(target,op,f->hi,g->hi));
	} else if (f->v < g->v ){
		return add_to_bdd(target,f->v,_meld(target,op,f->lo,g),_meld(target,op,f->hi,g));
	} else {
		return add_to_bdd(target,g->v,_meld(target,op,f,g->lo),_meld(target,op,f,g->hi));
	}
}

int meld(bdd *target, enum BOOL_OP op, bdd *f, bdd *g) {
	printf("Combining the following BDDS:\n");
	printf("==================BDD 1:\n");
	print_bdd(f);
	printf("==================BDD 2:\n");
	print_bdd(g);
	return _meld(target,op,&f->I[f->count-1],&g->I[g->count-1]);
}

//Utility functions






int make_dot(bdd *base, char *path) {
	if(strlen(path) == 0)
		return 0;
	FILE *out = fopen(path,"w");
	fprintf(out,"digraph bdd {\n");
	fprintf(out,"\tnode [shape=record];\n");
	fprintf(out,"\tnode%p [label = \"{ %s | { <v> %d|<lo> %p|<hi> %p| %d } }\"];\n",base->I,"False",base->I[0].v,base->I[0].lo,base->I[0].hi,base->I[0].aux);
	fprintf(out,"\tnode%p [label = \"{ %s | { <v> %d|<lo> %p|<hi> %p| %d } }\"];\n",base->I + sizeof(bdd_node),"True",base->I[1].v,base->I[1].lo,base->I[1].hi,base->I[1].aux);

	for(int i = 2;i<base->count;i++) {
		fprintf(out,"\tnode%p [label = \"{ %d | { <v> %d|<lo> %p|<hi> %p } }\"];\n",base->I + i * sizeof(bdd_node),i,base->I[i].v,base->I[i].lo,base->I[i].hi);
	}
	for(int i = 2;i<base->count;i++) {
		if(base->I[i].hi >= 0)
		fprintf(out,"\tnode%p:hi -> node%p [color=blue];\n",base->I + i * sizeof(bdd_node),base->I[i].hi);
		if(base->I[i].lo >= 0)
		fprintf(out,"\tnode%p:lo -> node%p [style=dashed color=red];\n",base->I + i * sizeof(bdd_node),base->I[i].lo);
	}
	fprintf(out,"}\n");
	fclose(out);
	return base->count;
}

int make_dot_r(bdd *base, char *path) {
	if(strlen(path) == 0)
		return 0;
	FILE *out = fopen(path,"w");
	fprintf(out,"digraph bdd {\n");
	fprintf(out,"\tnode [shape=record];\n");
	for(int i = 0;i<base->count;i++) {
		fprintf(out,"\tnode%d [label = \"{ %d | { <v> %d|<lo> %p|<hi> %p| %d } }\"];\n",i,i,base->I[i].v,base->I[i].lo,base->I[i].hi,base->I[i].aux);
	}
	for(int i = 2;i<base->count;i++) {
		if(base->I[i].hi >= 0)
		fprintf(out,"\tnode%d:hi -> node%p [color=blue];\n",i,base->I[i].hi);
		if(base->I[i].lo >= 0)
		fprintf(out,"\tnode%d:lo -> node%p [style=dashed color=red];\n",i,base->I[i].lo);
	}
	fprintf(out,"}\n");
	fclose(out);
	return base->count;
}

int make_simple_dot(bdd *base, char *path) {
	if(strlen(path) == 0)
		return 0;
	FILE *out = fopen(path,"w");
	fprintf(out,"digraph bdd{\n");
	fprintf(out,"\tnode%p [label = \"%s\"];\n",base->I,"False");
	fprintf(out,"\tnode%p [label = \"%s\"];\n",base->I + sizeof(bdd_node),"True");

	for(int i = 2;i<base->count;i++) {
		fprintf(out,"\tnode%p [label = %d];\n",base->I + i * sizeof(bdd_node),get_node(base,i).v);
	}
	for(int i = 2;i<base->count;i++) {
		if(base->I[i].hi >= 0)
			fprintf(out,"\tnode%p -> node%p [color=blue];\n",base->I + i * sizeof(bdd_node),base->I[i].hi);
		if(base->I[i].lo >= 0)
			fprintf(out,"\tnode%p -> node%p [style=dashed color=red];\n",base->I + i * sizeof(bdd_node),base->I[i].lo);
	}
	fprintf(out,"}\n");
	fclose(out);
	return base->count;
}
