#ifndef _BDD_HEADER
#define _BDD_HEADER

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

struct bdd_node;
struct bdd;
struct bdd_hash_table;
struct bdd_hash_entry;
//enum BOOL_OP;

typedef struct bdd_node bdd_node;
typedef struct bdd bdd;
typedef struct bdd_hash_table bdd_hash_table;
typedef struct bdd_hash_entry bdd_hash_entry;

int add_to_bdd(bdd *base, int v, int lo, int hi);
void new_bdd(bdd **in, int terms);
int get_bdd_count(bdd *base);
int make_dot(bdd *base, char *path);
int make_simple_dot(bdd *base, char *path);
int meld(bdd *target, enum BOOL_OP op, bdd *f, bdd *g);
#endif