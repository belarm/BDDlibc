#include <stdio.h>
#include "bddlib.h"

int main(int argc, char **argv)
{
	//hash_node *hashtable;// = get_new_hashtable();

/*	hash_table hashtable;
	bdd_node *start = malloc(sizeof(bdd_node));
	bdd_node *end = malloc(sizeof(bdd_node));
	bdd_node *dupe = malloc(sizeof(bdd_node));
	start->hi = 0;
	start->lo = 1;
	start->v = 10;
	end->hi=1;
	end->lo=1;
	end->v=10;
	dupe->v = end->v;
	dupe->hi = 2;
	dupe->lo = end->lo;*/


/*	hash_node one;
	one.node = &start;
	one.next = NULL;
	hashtable[bdd_hash(start)] = one;*/
/*	printf("res 1 : %d\n",add_to_hashtable(&hashtable,start));
	printf("res 2 : %d\n",add_to_hashtable(&hashtable,start));
	printf("res 3 : %d\n",add_to_hashtable(&hashtable,end));
	printf("res 4 : %d\n",add_to_hashtable(&hashtable,end));
	printf("res 5 : %d\n",add_to_hashtable(&hashtable,dupe));
	printf("HashTable has %d entries\n",hashtable.count);*/
	//free(start);
	BDD *bdd;
	BDD *bdd2;
	BDD *bdd3;
	BDD *Or;
	BDD *Nand;
	BDD *Result;
	//BDD 
	new_bdd(&bdd,4);
	new_bdd(&bdd2,4);
	new_bdd(&bdd3,4);
	new_bdd(&Or,2);
	new_bdd(&Nand,2);
	new_bdd(&Result,2);
	add_to_bdd(Or,1,0,1);
	add_to_bdd(Or,0,2,1);
	add_to_bdd(Nand,1,1,0);
	add_to_bdd(Nand,0,1,2);
	//add_to_bdd(bdd,1,0,1);
	//add_to_bdd(bdd,2,2,1);
	add_to_bdd(bdd,3,0,1);
	add_to_bdd(bdd,2,2,1);
	add_to_bdd(bdd,1,0,3);
	add_to_bdd(bdd,0,4,3);
	
	add_to_bdd(bdd2,3,0,1);
	add_to_bdd(bdd2,3,1,0);
	add_to_bdd(bdd2,2,2,3);
	add_to_bdd(bdd2,1,4,1);
	add_to_bdd(bdd2,1,1,4);
	add_to_bdd(bdd2,0,5,6);
	meld(Result,AND,Or,Nand);
	meld(bdd3,AND,bdd,bdd2);
	printf("B(f): %d\n",get_bdd_count(bdd));
	printf("B(g): %d\n",get_bdd_count(bdd2));
	printf("B(merge): %d\n",get_bdd_count(bdd3));
	printf("B(Or): %d\n",get_bdd_count(Or));
	printf("B(Nand): %d\n",get_bdd_count(Nand));
	printf("B(Result): %d\n",get_bdd_count(Result));
	printf("BDD\n");
	print_bdd(bdd);
	printf("BDD2\n");
	print_bdd(bdd2);
	printf("MERGE\n");
	print_bdd(bdd3);
	make_dot(bdd,"bdd.dot");
	make_dot(bdd2,"bdd2.dot");
	make_dot(bdd3,"merge.dot");
	printf("OR\n");
	print_bdd(Or);
	printf("NAND\n");
	print_bdd(Nand);
	printf("Result\n");
	print_bdd(Result);
	make_dot(Or,"or.dot");
	make_dot(Nand,"nand.dot");
	make_dot(Result,"xor6.dot");
	return 0;
}
