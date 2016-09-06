#include <bdd.h>
#include <string.h>
#include <stdlib.h>

static int width, height;

struct bdd_point {
	int x;
	int y;
	bdd f; //result of all edge constraints - uniquely identifies the point's function
	bdd v[8]; //holds variables
	bdd nv[8]; //holds inverse of variables
};

static struct bdd_point *map;

bdd get_link(int x, int y, int dir);
struct bdd_point *get_point(int x, int y);

struct bit8 {
	unsigned int b0 : 1;
	unsigned int b1 : 1;
	unsigned int b2 : 1;
	unsigned int b3 : 1;
	unsigned int b4 : 1;
	unsigned int b5 : 1;
	unsigned int b6 : 1;
	unsigned int b7 : 1;
};

union truth_entry {
	struct bit8 b;
	unsigned char i;
};

struct bdd_point new_point(int x, int y) {
	static int varcounter = 0;
	struct bdd_point ret;
	ret.x = x;
	ret.y = y;
	for(int i =0;i<8;i++) {
		ret.v[i] = bdd_ithvar(varcounter+i);
		ret.nv[i] = bdd_nithvar(varcounter+i);
	}
	varcounter += 8;
	bdd *v = ret.v;
	bdd a[28];

	int counter = 0;
	//REALY??????
	
	//a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];
	
	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & ret.v[6] & ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & ret.v[5] & !ret.v[6] & ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & ret.v[4] & !ret.v[5] & !ret.v[6] & ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & ret.v[7];
	a[counter++] = !ret.v[0] & ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & ret.v[7];
	a[counter++] = ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & ret.v[7];

	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & ret.v[5] & ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & ret.v[4] & !ret.v[5] & ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & ret.v[3] & !ret.v[4] & !ret.v[5] & ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & ret.v[6] & !ret.v[7];
	a[counter++] = ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & ret.v[6] & !ret.v[7];

	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & ret.v[4] & ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & ret.v[3] & !ret.v[4] & ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & ret.v[2] & !ret.v[3] & !ret.v[4] & ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & ret.v[5] & !ret.v[6] & !ret.v[7];


	a[counter++] = !ret.v[0] & !ret.v[1] & !ret.v[2] & ret.v[3] & ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & !ret.v[1] & ret.v[2] & !ret.v[3] & ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & ret.v[1] & !ret.v[2] & !ret.v[3] & ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = ret.v[0] & !ret.v[1] & !ret.v[2] & !ret.v[3] & ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];

	a[counter++] = !ret.v[0] & !ret.v[1] & ret.v[2] & ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = !ret.v[0] & ret.v[1] & !ret.v[2] & ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = ret.v[0] & !ret.v[1] & !ret.v[2] & ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];

	a[counter++] = !ret.v[0] & ret.v[1] & ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];
	a[counter++] = ret.v[0] & !ret.v[1] & ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];

	a[counter++] = ret.v[0] & ret.v[1] & !ret.v[2] & !ret.v[3] & !ret.v[4] & !ret.v[5] & !ret.v[6] & !ret.v[7];

	//seed logic
//	ret.f = !v[0] & !v[1] & !v[2] & !v[3] & !v[4] & !v[5] & !v[6] & !v[7];
	ret.f =
		a[0] | a[1] | a[2] | a[3] | a[4] | a[5] | a[6] |
		a[7] | a[8] | a[9] | a[10] | a[11] | a[12] | a[13] |
		a[14] | a[15] | a[16] | a[17] | a[18] | a[19] | a[20] |
		a[21] | a[22] | a[23] | a[24] | a[25] | a[26] | a[27];
	return ret;
}

const char *byte_to_binary(int x)
{
	static char b[9];
	b[0] = '\0';
	int z;
	for (z = 128; z > 0; z >>= 1)
	{
		strcat(b, ((x & z) == z) ? "1" : "0");
	}
	return b;
}



int main(int argc, char *argv[])
{
	if(argc < 3) {
		printf("USAGE: %s width height\n",argv[0]);
		return 1;
	}
	width = atoi(argv[1]);
	height = atoi(argv[2]);
	int nodefactor = 12;
	int nodecount = width * height * (1 << nodefactor) + 2;
	printf("allocating %d initial nodes\n",nodecount);
	//bdd_init(nodecount + 10,10000);
	bdd_init(1000,10000);
	bdd_setvarnum(width * height * 8);
	map = (struct bdd_point *)malloc(width * height * sizeof(struct bdd_point));
	bdd fullmap = bddfalse;
	int maxnodes = 0;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			map[x + y * width] = new_point(x,y);
	//		printf("Adding point (%d,%d)\n",x,y);
			//maxnodes = bdd_getnodenum() > maxnodes ? bdd_getnodenum() : maxnodes;
			//fullmap = bdd_apply(fullmap,map[x + y * width].f,bddop_or);
		}
	}
	//add biimplications
	bdd_fnprintdot((char *)"fullmap-s0.dot",fullmap);
	int dx[] = {1,1,0,-1,-1,-1,0,1};
	int dy[] = {0,1,1,1,0,-1,-1,-1};
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			for(int v = 0;v<8;v++) {
				int x2 = x+dx[v];
				int y2 = y+dy[v];
				int dir = (v+4) % 8;
				struct bdd_point *left = &map[x + y * width];
				struct bdd_point *right = get_point(x2,y2);
				printf("linking (%d,%d).%d <==> (%d,%d).%d\n",x,y,v,x2,y2,dir);
				//bdd left = get_link(x,y,v);
				//bdd right = get_link(x2,y2,dir);
				if(right == NULL) {
					//fullmap = bdd_restrict(fullmap,map[x + y * width].nv[v]);
					left->f = bdd_restrict(left->f,left->nv[v]);
				} else {
					bdd linkrelation = bdd_apply(left->v[v],right->v[dir],bddop_biimp);
					left->f = bdd_constrain(left->f,linkrelation);
				}

				fullmap = bdd_apply(fullmap,left->f,bddop_or);
				//bdd linkrelation = bdd_apply(get_link(x,y,v),get_link(x2,y2,dir),bddop_biimp);
				//				fullmap = bdd_constrain(fullmap,bdd_apply(get_link(x,y,v),get_link(x + dx[v], y + dy[v], v + 4 % 8),bddop_biimp));
				//printf("\n");
				//fullmap = bdd_constrain(fullmap,linkrelation);
			}
		}
	}
	
	printf("Got %d total nodes, %d max\n",bdd_getnodenum(),maxnodes);	
	printf("------Beginning path count\n");
	printf("map has %d paths\n",bdd_satcount(fullmap));
	bdd_fnprintdot((char *)"fullmap.dot",fullmap);
	//bdd_fnprintdot((char *)"point0.dot",point0.f);
	bdd_printall();
	free(map);
	bdd_done();
	return 0;
}

struct bdd_point *get_point(int x, int y) {
	if(x < 0 || y < 0 || x >= width || y >= height) {
		return NULL;
	} else {
		return &map[x+y*width];
	}
}

bdd get_link(int x, int y, int dir) {
	if(x < 0 || y < 0 || x >= width || y >= height) {
		printf("F  ");
		return bddfalse;
	}
	else {
		printf("OK ");
	//	printf("Returning (%d,%d).%d\n",x,y,dir);
		return map[x + y * width].v[dir];
	}
}
