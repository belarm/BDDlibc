#include <bdd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <math.h>

#define PVARS 9

static int width, height;
struct bdd_point {
		int x,y;
		bdd f;
		bdd pointvar;
		bdd v[8];
};

typedef struct bdd_point bdd_point;

//bdd_point *map;
bdd link(int x,int y, int dir);
//bdd_point *get_point(int x, int y);
bdd make_constraint();
bdd_point make_point(int x, int y, int startvar, bdd function);
void satGraph(char *varset, int size);

void satPrinter(char *varset, int size);
int maxindex;
int dx[] = {1,1,0,-1,-1,-1,0,1};
int dy[] = {0,1,1,1,0,-1,-1,-1};


int main(int argc, char *argv[]) {
	if(argc < 3) {
		printf("USAGE: %s width height\n",argv[0]);
		return 1;
	}
	width = atoi(argv[1]);
	height = atoi(argv[2]);
	if (width < 1 or height < 1)
		return 0;
	maxindex = width * height - 1;
	int varcount = width * height * PVARS; //9 vars per point
	//map = (bdd_point *)malloc(width * height * sizeof(bdd_point));
	

	
	bdd_init(10000000,100000);
	bdd_setvarnum(varcount);

	bdd edgerules = make_constraint();
	bddPair *pair = bdd_newpair();

	bdd mapfunc = bddtrue;
	char buf[1024];
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int address = x + y*width;
			bdd_resetpair(pair);
			for(int i = 0; i < PVARS; i++) {
				bdd_setpair(pair,i,i+address*PVARS);
			}
			bdd pointfunc = bdd_replace(edgerules,pair);
			for(int i = 0; i < 8; i++) {
				bdd a,b;
				a = link(x,y,i);
				b = link(x+dx[i],y+dy[i],(i+4) % 8);
				pointfunc = pointfunc & bdd_apply(a,b,bddop_imp);
			}
			mapfunc = mapfunc & pointfunc;
			sprintf(buf,"point_%d_%d.dot",x,y);
			bdd_fnprintdot(buf,pointfunc);
		}
	}

	printf("Solutions:\n");
	bdd_allsat(mapfunc, *satGraph); //1,1
	bdd_fnprintdot((char *)"mapfunc.dot",mapfunc);
	bdd_reorder(BDD_REORDER_WIN3);
	bdd_fnprintdot((char *)"mapfunc-reordered.dot",mapfunc);
	printf("Map has %d paths\n",bdd_satcount(mapfunc));
	printf("Map has %d paths\n",bdd_pathcount(mapfunc));
	bdd_done();
	return 0;
}

bdd_point make_point(int x, int y, int startvar, bdd function) {
	bdd_point ret;
	ret.x = x;
	ret.y = y;
	ret.pointvar = bdd_ithvar(startvar++);
	for(int i =0;i<8; i++)
		ret.v[i] = bdd_ithvar(startvar + i);
	ret.f = function;
	return ret;
}

void satGraph(char *varset, int size) {
	static int c = 0;
	char buf[100];
	sprintf(buf,"solution_%d.svg",c++);
	//printf("%s\n",buf);
	cairo_surface_t *svg = cairo_svg_surface_create(buf,width*10,height*10);
	cairo_svg_surface_restrict_to_version(svg,CAIRO_SVG_VERSION_1_2);
	cairo_t *cr = cairo_create(svg);
	cairo_set_line_width(cr,1);

	for(int i = 0;i<size;i++) {
		if(varset[i]) {
			int x1 = ((i / PVARS) % width) * 10 + 5;
			int y1 = (i / PVARS / width) * 10 + 5;
			if(i % PVARS == 0) {
				cairo_set_source_rgba(cr,0,0,0,.5);
				cairo_arc(cr,x1,y1,1,0,2*M_PI);
				cairo_fill(cr);
				cairo_stroke(cr);
			} else {
//				int x1 = (i / PVARS) % width;
//				int y1 = i / PVARS / width;
				int e = i % PVARS;
				int x2 = ((i / PVARS) % width) + dx[e-1];
				int y2 = i / PVARS / width + dy[e-1];
				x2 = x2 * 10 + 5;
				y2 = y2 * 10 + 5;
				cairo_set_source_rgba(cr,0,0,1,.5);
				cairo_move_to(cr,x1,y1);
				cairo_line_to(cr,x2,y2);
				cairo_stroke(cr);
			}
//			printf("(%d,%d)%s(%d,%d),   ",x1,y1,varset[i] > 0 ? "-" : "x",x2,y2);
		}
	}
	cairo_surface_flush(svg);
	cairo_surface_finish(svg);
	cairo_surface_destroy(svg);
	cairo_destroy(cr);
//	printf("\n");
}

void satPrinter(char *varset, int size) {
	for(int v=0;v<size;v++) {
		if(v % PVARS == 0)
			putchar(' ');
		printf("%c",varset[v] < 0 ? 'X' : (char)('0' + varset[v]));
	}
	printf("\n");
}

bdd make_constraint() {
	bdd f = bddfalse; //start function
	bdd p = (bdd_ithvar(0));			//point status
	bdd e1 = (bdd_ithvar(1));		//inverse of associated edges
	bdd e2 = (bdd_ithvar(2));
	bdd e3 = (bdd_ithvar(3));
	bdd e4 = (bdd_ithvar(4));
	bdd e5 = (bdd_ithvar(5));
	bdd e6 = (bdd_ithvar(6));
	bdd e7 = (bdd_ithvar(7));
	bdd e8 = (bdd_ithvar(8));
	bdd noedge = bddtrue;
	noedge = (bdd_apply(noedge,!e1,bddop_and));
	noedge = (bdd_apply(noedge,!e2,bddop_and));
	noedge = (bdd_apply(noedge,!e3,bddop_and));
	noedge = (bdd_apply(noedge,!e4,bddop_and));
	noedge = (bdd_apply(noedge,!e5,bddop_and));
	noedge = (bdd_apply(noedge,!e6,bddop_and));
	noedge = (bdd_apply(noedge,!e7,bddop_and));
	noedge = (bdd_apply(noedge,!e8,bddop_and));

//	f = (bdd_apply(f,p,bddop_or));
	f = (bdd_apply(f,noedge,bddop_or));


	bdd t00 = !e1 & !e2 & !e3 & !e4 & !e5 & !e6 & !e7 & !e8;
	
	bdd t01 = e1 & e2 & !e3 & !e4 & !e5 & !e6 & !e7 & !e8;
	bdd t02 = e1 & !e2 & e3 & !e4 & !e5 & !e6 & !e7 & !e8;
	bdd t03 = e1 & !e2 & !e3 & e4 & !e5 & !e6 & !e7 & !e8;
	bdd t04 = e1 & !e2 & !e3 & !e4 & e5 & !e6 & !e7 & !e8;	
	bdd t05 = e1 & !e2 & !e3 & !e4 & !e5 & e6 & !e7 & !e8;
	bdd t06 = e1 & !e2 & !e3 & !e4 & !e5 & !e6 & e7 & !e8;
	bdd t07 = e1 & !e2 & !e3 & !e4 & !e5 & !e6 & !e7 & e8;

	bdd t08 = !e1 & e2 & e3 & !e4 & !e5 & !e6 & !e7 & !e8;
	bdd t09 = !e1 & e2 & !e3 & e4 & !e5 & !e6 & !e7 & !e8;
	bdd t10 = !e1 & e2 & !e3 & !e4 & e5 & !e6 & !e7 & !e8;
	bdd t11 = !e1 & e2 & !e3 & !e4 & !e5 & e6 & !e7 & !e8;
	bdd t12 = !e1 & e2 & !e3 & !e4 & !e5 & !e6 & e7 & !e8;
	bdd t13 = !e1 & e2 & !e3 & !e4 & !e5 & !e6 & !e7 & e8;

	bdd t14 = !e1 & !e2 & e3 & e4 & !e5 & !e6 & !e7 & !e8;
	bdd t15 = !e1 & !e2 & e3 & !e4 & e5 & !e6 & !e7 & !e8;
	bdd t16 = !e1 & !e2 & e3 & !e4 & !e5 & e6 & !e7 & !e8;
	bdd t17 = !e1 & !e2 & e3 & !e4 & !e5 & !e6 & e7 & !e8;
	bdd t18 = !e1 & !e2 & e3 & !e4 & !e5 & !e6 & !e7 & e8;

	bdd t19 = !e1 & !e2 & !e3 & e4 & e5 & !e6 & !e7 & !e8;
	bdd t20 = !e1 & !e2 & !e3 & e4 & !e5 & e6 & !e7 & !e8;
	bdd t21 = !e1 & !e2 & !e3 & e4 & !e5 & !e6 & e7 & !e8;
	bdd t22 = !e1 & !e2 & !e3 & e4 & !e5 & !e6 & !e7 & e8;

	bdd t23 = !e1 & !e2 & !e3 & !e4 & e5 & e6 & !e7 & !e8;
	bdd t24 = !e1 & !e2 & !e3 & !e4 & e5 & !e6 & e7 & !e8;
	bdd t25 = !e1 & !e2 & !e3 & !e4 & e5 & !e6 & !e7 & e8;

	bdd t26 = !e1 & !e2 & !e3 & !e4 & !e5 & e6 & e7 & !e8;
	bdd t27 = !e1 & !e2 & !e3 & !e4 & !e5 & e6 & !e7 & e8;

	bdd t28 = !e1 & !e2 & !e3 & !e4 & !e5 & !e6 & e7 & e8;
	bdd limit = p & (t00 | t01 | t02 | t03 | t04 | t05 | t06 | t07 |
						  t08 | t09 | t10 | t11 | t12 | t13 | t14 | t15 |
						  t16 | t17 | t18 | t19 | t20 | t21 | t22 | t23 |
						  t24 | t25 | t26 | t27 | t28 );
	f = f | limit;
	f = f & bdd_apply(e1,p,bddop_imp);
	f = f & bdd_apply(e2,p,bddop_imp);
	f = f & bdd_apply(e3,p,bddop_imp);
	f = f & bdd_apply(e4,p,bddop_imp);
	f = f & bdd_apply(e5,p,bddop_imp);
	f = f & bdd_apply(e6,p,bddop_imp);
	f = f & bdd_apply(e7,p,bddop_imp);
	f = f & bdd_apply(e8,p,bddop_imp);
	f = f & bdd_apply(p,e1|e2|e3|e4|e5|e6|e7|e8,bddop_imp);
	return f;
}

bdd link(int x,int y, int dir) {
	if(x < 0 || y < 0 || x >= width || y >= height) {
		return bddfalse;
	} else {
		return bdd_ithvar((x + y * width) * 9 + dir + 1);
	}
}

bdd get_link(int addr) {
//	printf("Returning %d,%d.%d"
	return bdd_ithvar(addr);
}

/*bdd_point *get_point(int x, int y) {
	if(x < 0 || y < 0 || x >= width || y >= height) {
	return NULL;
	} else {
		return &map[x+y*width];
	}
}*/
