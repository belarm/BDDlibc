#ifndef _BDD_HEADER
#define _BDD_HEADER

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>





//Each boolean op is represented by an integer that matches
//its truth table. This allows construction of boolean expressions by
//bitwise operations.
enum BOOL_OP {
	FALSE,
	AND,
	A_AND_NOT_B,
	A,
	NOT_A_AND_B,
	B,
	XOR,
	OR,
	NOR,
	XNOR,
	NOT_B,
	A_OR_NOT_B,
	NOT_A,
	NOT_A_OR_B,
	NAND,
	TRUE
};



struct BDD;
struct bdd_node_s;
struct hash_node_s;
struct hash_table_s;
enum BOOL_OP;
union packed_op;


typedef struct BDD BDD;
typedef struct bdd_node_s bdd_node;
typedef struct hash_node_s hash_node;
typedef struct hash_table_s hash_table;


bool nodes_equal(bdd_node ,bdd_node );


struct BDD;
typedef struct BDD BDD;

int get_bdd_count(BDD *bdd);
int add_to_bdd(BDD *bdd, int v, int lo, int hi);
BDD *new_bdd(BDD **bdd, int terms);
int get_bdd_count(BDD *bdd);
int print_bdd(BDD *bdd);
int make_dot(BDD *bdd, char *path);
int make_simple_dot(BDD *bdd, char *path);
int make_dot_r(BDD *bdd, char *path);
int KnuthR(BDD *dag, int root);

int meld (BDD *target, enum BOOL_OP op, BDD *f, BDD *g);
int make_dot(bdd *base, char *path);
int make_dot_r(bdd *base, char *path);
int make_simple_dot(bdd *base, char *path);




#endif /* _BDD_HEADER */