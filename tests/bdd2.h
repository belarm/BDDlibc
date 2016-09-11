#ifndef _BDD_HEADER
#define _BDD_HEADER

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct bdd_node;
struct bdd;
struct bdd_hash_table;
struct bdd_hash_entry;
enum BOOL_OP;

typedef struct bdd_node bdd_node;
typedef struct bdd bdd;
typedef struct bdd_hash_table bdd_hash_table;
typedef struct bdd_hash_entry bdd_hash_entry;

#endif