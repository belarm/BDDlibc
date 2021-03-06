#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "bddlib.h"

#define HASH_SIZE 24593
#define _get_truth(X) (X.hi == 1)
#define _is_sink(X) ((X).v == (X).bdd->terms)


struct bdd_node_s {
	int v;
	int lo;
	int hi;
	int aux;
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

//bdd_node get_node(BDD *bdd, int node);

uint bdd_hash(bdd_node node) {
	int ret = 1;
	ret = ret * 31 + node.v;
	ret = ret * 31 + node.hi;
	return (ret * 31 + node.lo) % HASH_SIZE;
}

int add_to_hashtable(bdd_node *node) {
	uint hash_val = bdd_hash(*node);
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

hash_node *get_new_hashtable() {
	return (hash_node *)malloc(sizeof(hash_node) * HASH_SIZE);
}

bdd_node _add_to_bdd(BDD *bdd, bdd_node node) {
	return bdd->I[add_to_bdd(bdd, node.v, node.lo, node.hi)];
}

int add_to_bdd(BDD *bdd, int v, int lo, int hi) {
	
	bdd_node temp = {.v = v, .lo = lo, .hi = hi, .bdd = bdd, .aux = 0};
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
	printf("Created BDD with %d nodes\n",ret->count);
	return *in;
}

bool nodes_equal(bdd_node a, bdd_node b) {
	return (a.v == b.v && a.hi == b.hi && a.lo == b.lo);
}

int get_bdd_count(BDD *bdd){
	return bdd->count;
}

int print_bdd(BDD *bdd) {
	int i;
	for(i=0;i<bdd->count;i++) {
		printf("%d\t%d?%d:%d\n",i,bdd->I[i].v,bdd->I[i].lo,bdd->I[i].hi);
	}
	return i;
}


bdd_node get_node(BDD *bdd, int node) {
	return bdd->I[node];
}

bdd_node *get_node_ref(BDD *bdd, int node, int *idx) {
//	printf("GETTING NODE\t%d\n",node);
//	printf("\t\t%d?%d:%d %c%d\n",bdd->I[node].v,bdd->I[node].lo,bdd->I[node].hi,bdd->I[node].aux >= 0 ? ' ' : '~', bdd->I[node].aux >= 0 ? bdd->I[node].aux : -bdd->I[node].aux - 1);
//	fflush(stdout);
	*idx = node;
	return &bdd->I[node];
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

int _meld (BDD *target, enum BOOL_OP op, bdd_node f, bdd_node g) {
	if(_is_sink(f) && _is_sink(g)) {
		return bdd_op(op, _get_truth(f), _get_truth(g));
	}
	if(f.v == g.v) {
		return add_to_bdd(target,f.v,_meld(target,op,get_lo(f),get_lo(g)),_meld(target,op,get_hi(f),get_hi(g)));
	} else if (f.v < g.v ){
		return add_to_bdd(target,f.v,_meld(target,op,get_lo(f),g),_meld(target,op,get_hi(f),g));
	} else {
		return add_to_bdd(target,g.v,_meld(target,op,f,get_lo(g)),_meld(target,op,f,get_hi(g)));
	}
}

int meld(BDD *target, enum BOOL_OP op, BDD *f, BDD *g) {
	return _meld(target,op,get_node(f,f->count-1),get_node(g,g->count-1));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Or, we can enforce the constraints as the DAG is built, like sane people.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int KnuthR(BDD *dag, int root) {
	int next;
	//~ int counter = 0;
	if(root < 2) 							//constant funnction
		return root;
	int s, r;
	bdd_node *p, *q;
	int pdx, qdx, avail;								//p's index in I
	int rv = get_node(dag,root).v;
	int head[dag->terms];				//access by [v-rv]
	//int aux[root+1];							//one int per node between root and FALSE
	//memset(aux,root+1,sizeof(bdd_node));
	//bool redundant = false;
	avail = root << 4;
	
	for(int k = 1; k <= dag->terms - rv; k++)
		head[k] = ~0;
	for(int k = 2; k < root;k++) {
		dag->I[k].aux = 0;
	}
	dag->I[0].aux = dag->I[1].aux = dag->I[root].aux = ~0;
	//R1
	s = root;
	while(s != 0) {
		printf("Getting node %d:\t",s);
		p = get_node_ref(dag,s,&pdx);
		printf("\tGot node: %d?%d:%d\n",p->v,p->lo,p->hi);
		s = ~p->aux;
		p->aux = head[p->v];
		head[p->v] = ~pdx;
		if(dag->I[p->lo].aux >= 0) {
			dag->I[p->lo].aux = ~s;
			s = p->lo;
		}
		if(dag->I[p->hi].aux >= 0) {
			dag->I[p->hi].aux = ~s;
			s = p->hi;
		}
	}
	//End R1 Passes check 1
	dag->I[0].aux = dag->I[1].aux = 0;
	for(int v = dag->terms;v > rv; v--) {
		p = get_node_ref(dag,~head[v],&pdx);
		s = 0;
		do {
			while(pdx != 0) {
				next = ~p->aux;
				q = get_node_ref(dag,p->hi, &qdx);
				if(q->lo < 0) // p.lo is dead
					p->hi = ~q->lo;
				
				q = get_node_ref(dag,p->lo, &qdx);
				if(q->lo < 0) {// p.hi is dead
					p->lo = ~q->lo;
					q = get_node_ref(dag,p->lo, &qdx);
				}
				if(p->lo == p->hi) { //hi and lo point to same node; safe because all nodes with .v > v have been reduced
					p->lo = ~qdx;
					p->hi = avail;
					p->aux = 0;
					avail = pdx;
				} else if(q->aux >= 0) {
					p->aux = s;
					s = ~qdx;
					q->aux = ~pdx;
				} else {
					p->aux = dag->I[~q->aux].aux;
					dag->I[~q->aux].aux = pdx;
				}
				p = get_node_ref(dag,next,&pdx);
			}
			//R3 done?
			//R4
			for (r=~s,s=0;r>0;r = ~p->aux) {
				printf("Chasing node %d in preparation for R6\n",r);
				q = get_node_ref(dag,~dag->I[r].aux,&qdx);
				dag->I[r].aux = 0;
				if(s == 0)
					s = qdx;
				else
					p->aux = qdx;
				p = q;
				pdx = qdx;
				while(p->aux > 0) {
					p = get_node_ref(dag,p->aux,&pdx);
				}
				r = ~p->aux;
			}
			//R5
			printf("Getting node %d in preparation for R6\n",s);
			p = get_node_ref(dag,s,&pdx);
			if(pdx != 0) {
				q = p;
				qdx = pdx;
				//R6
				while(pdx != 0) {
					printf("Entering R6\n");
					s = p->lo;
					//Remove duplicates. Apparently.
					//R7
					do {
						r = q->hi;
						if(dag->I[r].aux >= 0) {
							printf("=================R7 - NOT deleting node %d\n",qdx);
							dag->I[r].aux = ~qdx;
						} else {
							printf("=================R7 - deleting node %d\n",qdx);
							q->lo = dag->I[r].aux;
							q->hi = avail;
							avail = qdx;
						}
						q = get_node_ref(dag, q->aux, &qdx);
					}while(qdx != 0 && q->lo == s); // Repeat R7
					//R8
					//More cleanup
					do {
						if(p->lo >= 0)
							dag->I[p->hi].aux = 0;
						p = get_node_ref(dag,p->aux,&pdx);
					} while (pdx != qdx);
				}
			}
		} while (pdx != 0);
		//R9
	}
	bdd_node rnode = get_node(dag,root);
	if(rnode.lo < 0) {
		return rnode.lo;
	} else
		return root;
}

//-----------------------------Utility functions-------------------------



int make_dot(BDD *bdd, char *path) {
	if(strlen(path) == 0)
		return 0;
	FILE *out = fopen(path,"w");
	fprintf(out,"digraph bdd {\n");
	fprintf(out,"\tnode [shape=record];\n");
	fprintf(out,"\tnode%d [label = \"{ %s | { <v> %d|<lo> %d|<hi> %d| %d } }\"];\n",0,"False",bdd->I[0].v,bdd->I[0].lo,bdd->I[0].hi,bdd->I[0].aux);
	fprintf(out,"\tnode%d [label = \"{ %s | { <v> %d|<lo> %d|<hi> %d| %d } }\"];\n",1,"True",bdd->I[1].v,bdd->I[1].lo,bdd->I[1].hi,bdd->I[1].aux);

	for(int i = 2;i<bdd->count;i++) {
		fprintf(out,"\tnode%d [label = \"{ %d | { <v> %d|<lo> %d|<hi> %d } }\"];\n",i,i,bdd->I[i].v,bdd->I[i].lo,bdd->I[i].hi);
	}
	for(int i = 2;i<bdd->count;i++) {
		if(bdd->I[i].hi >= 0)
		fprintf(out,"\tnode%d:hi -> node%d [color=blue];\n",i,bdd->I[i].hi);
		if(bdd->I[i].lo >= 0)
		fprintf(out,"\tnode%d:lo -> node%d [style=dashed color=red];\n",i,bdd->I[i].lo);
	}
	fprintf(out,"}\n");
	fclose(out);
	return bdd->count;
}

int make_dot_r(BDD *bdd, char *path) {
	if(strlen(path) == 0)
		return 0;
	FILE *out = fopen(path,"w");
	fprintf(out,"digraph bdd {\n");
	fprintf(out,"\tnode [shape=record];\n");
	for(int i = 0;i<bdd->count;i++) {
		fprintf(out,"\tnode%d [label = \"{ %d | { <v> %d|<lo> %d|<hi> %d| %d } }\"];\n",i,i,bdd->I[i].v,bdd->I[i].lo,bdd->I[i].hi,bdd->I[i].aux);
	}
	for(int i = 2;i<bdd->count;i++) {
		if(bdd->I[i].hi >= 0)
		fprintf(out,"\tnode%d:hi -> node%d [color=blue];\n",i,bdd->I[i].hi);
		if(bdd->I[i].lo >= 0)
		fprintf(out,"\tnode%d:lo -> node%d [style=dashed color=red];\n",i,bdd->I[i].lo);
	}
	fprintf(out,"}\n");
	fclose(out);
	return bdd->count;
}



int make_simple_dot(BDD *bdd, char *path) {
	if(strlen(path) == 0)
		return 0;
	FILE *out = fopen(path,"w");
	fprintf(out,"digraph bdd {\n");
	fprintf(out,"\tnode%d [label = \"%s\"];\n",0,"False");
	fprintf(out,"\tnode%d [label = \"%s\"];\n",1,"True");

	for(int i = 2;i<bdd->count;i++) {
		fprintf(out,"\tnode%d [label = %d];\n",i,get_node(bdd,i).v);
	}
	for(int i = 2;i<bdd->count;i++) {
		if(bdd->I[i].hi >= 0)
			fprintf(out,"\tnode%d -> node%d [color=blue];\n",i,bdd->I[i].hi);
		if(bdd->I[i].lo >= 0)
			fprintf(out,"\tnode%d -> node%d [style=dashed color=red];\n",i,bdd->I[i].lo);
	}
	fprintf(out,"}\n");
	fclose(out);
	return bdd->count;
}
