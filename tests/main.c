#include <stdio.h>
#include <stdlib.h>
#include "bddlib.h"

#define _GRAPH_WIDTH 3
#define _GRAPH_HEIGHT 3
//4xy - 3x - 3y + 2 edges in an 8-connected grid graph x [X] y
//#define _EDGES 4 * _GRAPH_HEIGHT * _GRAPH_WIDTH - 3 * _GRAPH_WIDTH - 3 * _GRAPH_HEIGHT + 2
//except not, because outward-facing edges are a thing
#define _EDGES 8 * _GRAPH_HEIGHT * _GRAPH_WIDTH

struct edge_s {
	int x1;
	int y1;
	int x2;
	int y2;
	int address;
};
typedef struct edge_s edge;


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
	edge *edges = (edge *)malloc(_EDGES * sizeof(edge));
	printf("Ready for %d edges\n",_EDGES);
	
	
	BDD *point;
	BDD *voxel;
	BDD *voxel2;
	BDD *vmerge;
	BDD *bdd;
	BDD *bdd2;
	BDD *bdd3;
	BDD *Or;
	BDD *Nand;
	BDD *Result;
	BDD *fig2;
	//BDD 
	new_bdd(&point,8);
	new_bdd(&voxel,8);
	new_bdd(&voxel2,8);
	new_bdd(&vmerge,8);
	new_bdd(&bdd,4);
	new_bdd(&bdd2,4);
	new_bdd(&bdd3,4);
	new_bdd(&Or,2);
	new_bdd(&Nand,2);
	new_bdd(&Result,2);
	new_bdd(&fig2,3);
	
	add_to_bdd(voxel,8,1,0);
	add_to_bdd(voxel,7,2,0);
	add_to_bdd(voxel,6,3,0);
	//add_to_bdd(voxel,5,4,0);
	add_to_bdd(voxel,4,4,0);
	add_to_bdd(voxel,3,5,0);
	add_to_bdd(voxel,2,6,0);
	add_to_bdd(voxel,1,7,0);
	
	add_to_bdd(voxel2,8,1,0);
	add_to_bdd(voxel2,7,2,0);
	add_to_bdd(voxel2,6,3,0);
	add_to_bdd(voxel2,5,4,0);
	add_to_bdd(voxel2,4,5,0);
	//add_to_bdd(voxel3,2,5,0);
	add_to_bdd(voxel2,2,6,0);
	add_to_bdd(voxel2,1,7,0);
	
	add_to_bdd(point,7,1,0);
	add_to_bdd(point,7,0,1);
	add_to_bdd(point,6,2,0);
	add_to_bdd(point,6,3,2);
	add_to_bdd(point,6,2,3);
	add_to_bdd(point,5,4,0);
	add_to_bdd(point,5,5,4);
	add_to_bdd(point,5,6,5);
	add_to_bdd(point,4,7,0);
	add_to_bdd(point,4,8,7);
	add_to_bdd(point,4,9,8);
	add_to_bdd(point,3,10,0);
	add_to_bdd(point,3,11,10);
	add_to_bdd(point,3,12,11);
	add_to_bdd(point,2,13,0);
	add_to_bdd(point,2,14,13);
	add_to_bdd(point,2,15,14);
	add_to_bdd(point,1,17,16);
	add_to_bdd(point,1,18,17);
	add_to_bdd(point,0,20,19);
	
	
	
	


	
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
	
	add_to_bdd(fig2,3,1,1);
	add_to_bdd(fig2,3,0,1);
	add_to_bdd(fig2,3,0,1);
	add_to_bdd(fig2,3,0,0);
	add_to_bdd(fig2,2,3,2);
	add_to_bdd(fig2,2,5,4);
	add_to_bdd(fig2,1,7,6);
	make_simple_dot(point,"point-unreduced.dot");
	KnuthR(point,6);
	make_simple_dot(point,"point-reduced.dot");
	make_dot(voxel,"voxel.dot");
	make_dot(voxel2,"voxel2.dot");
	make_simple_dot(voxel,"voxel_simple.dot");
	make_simple_dot(voxel2,"voxel2_simple.dot");
	meld(vmerge,OR,voxel,voxel2);
	make_dot(vmerge,"vmerge.dot");
	make_simple_dot(vmerge,"vmerge_simple.dot");
	KnuthR(vmerge,17);
	make_dot(vmerge,"vmerge_reduced.dot");
	make_simple_dot(vmerge,"vmerge_reduced_simple.dot");
	make_dot(fig2,"figure2.dot");
	make_simple_dot(fig2,"figure2_simple.dot");

	KnuthR(fig2,8);
	
	
	
	meld(Result,AND,Or,Nand);
	make_dot(Result,"xor6-merged.dot");
	KnuthR(Result,2);
	
	meld(bdd3,AND,bdd,bdd2);
/*	printf("B(f): %d\n",get_bdd_count(bdd));
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
	print_bdd(bdd3); */
	//make_dot(bdd,"bdd.dot");
	//make_dot(bdd2,"bdd2.dot");
	//make_dot(bdd3,"merge.dot");
/*	printf("OR\n");
	print_bdd(Or);
	printf("NAND\n");
	print_bdd(Nand);
	printf("Result\n");
	print_bdd(Result); */
	//make_dot(Or,"or.dot");
	//make_dot(Nand,"nand.dot");
	make_dot(Result,"xor6-reduced.dot");
	make_dot(fig2,"figure2-reduced.dot");
	make_simple_dot(fig2,"figure2_simple-reduced.dot");
	free(edges);
	return 0;
}
