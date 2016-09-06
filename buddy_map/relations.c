#include <bdd.h>
#include <stdio.h>
#include <stdlib.h>
bdd make_constraint();
void satPrinter(char *varset, int size);

int main(int argc, char *argv[]) {
	bdd_init(10000,1000);
	bdd_setvarnum(9 + 9);
	bdd pf = make_constraint();
	bddPair *pair = bdd_newpair();
	for(int i = 0;i<9;i++)
		bdd_setpair(pair,i,i+9);
	bdd pf2 = bdd_replace(pf,pair);
	pf = pf & bdd_nithvar(2);
	pf = pf & bdd_nithvar(3);
//	pf = pf & bdd_nithvar(4);
	pf = pf & bdd_nithvar(5);
	pf = pf & bdd_nithvar(6);
	pf = pf & bdd_nithvar(7);
	pf = pf & bdd_nithvar(8);
	
	pf2 = pf2 & bdd_nithvar(10);
	pf2 = pf2 & bdd_nithvar(11);
	pf2 = pf2 & bdd_nithvar(12);
	pf2 = pf2 & bdd_nithvar(13);
	pf2 = pf2 & bdd_nithvar(15);
	pf2 = pf2 & bdd_nithvar(16);
	pf2 = pf2 & bdd_nithvar(17);
	printf("Point1 solutions: \n");
	bdd_allsat(pf,*satPrinter);
	printf("Point2 solutions: \n");
	bdd_allsat(pf2,*satPrinter);

	bdd_fnprintdot((char *)"point1.dot",pf);
	bdd_fnprintdot((char *)"point2.dot",pf2);


	bdd_done();
	return 0;
}

void satPrinter(char *varset, int size) {
	for(int v=0;v<size;v++) {
		printf("%c",varset[v] < 0 ? 'X' : (char)('0' + varset[v]));
	}
	printf("\n");
}

bdd make_constraint() {
	bdd np = (bdd_nithvar(0));			//point status
	bdd ne1 = (bdd_nithvar(1));		//inverse of associated edges
	bdd ne2 = (bdd_nithvar(2));
	bdd ne3 = (bdd_nithvar(3));
	bdd ne4 = (bdd_nithvar(4));
	bdd ne5 = (bdd_nithvar(5));
	bdd ne6 = (bdd_nithvar(6));
	bdd ne7 = (bdd_nithvar(7));
	bdd ne8 = (bdd_nithvar(8));
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
	noedge = (bdd_apply(noedge,ne1,bddop_and));
	noedge = (bdd_apply(noedge,ne2,bddop_and));
	noedge = (bdd_apply(noedge,ne3,bddop_and));
	noedge = (bdd_apply(noedge,ne4,bddop_and));
	noedge = (bdd_apply(noedge,ne5,bddop_and));
	noedge = (bdd_apply(noedge,ne6,bddop_and));
	noedge = (bdd_apply(noedge,ne7,bddop_and));
	noedge = (bdd_apply(noedge,ne8,bddop_and));

//	f = (bdd_apply(f,p,bddop_or));
	f = (bdd_apply(f,noedge,bddop_or));


	bdd t00 = ne1 & ne2 & ne3 & ne4 & ne5 & ne6 & ne7 & ne8;
	
	bdd t01 = e1 & e2 & ne3 & ne4 & ne5 & ne6 & ne7 & ne8;
	bdd t02 = e1 & ne2 & e3 & ne4 & ne5 & ne6 & ne7 & ne8;
	bdd t03 = e1 & ne2 & ne3 & e4 & ne5 & ne6 & ne7 & ne8;
	bdd t04 = e1 & ne2 & ne3 & ne4 & e5 & ne6 & ne7 & ne8;	
	bdd t05 = e1 & ne2 & ne3 & ne4 & ne5 & e6 & ne7 & ne8;
	bdd t06 = e1 & ne2 & ne3 & ne4 & ne5 & ne6 & e7 & ne8;
	bdd t07 = e1 & ne2 & ne3 & ne4 & ne5 & ne6 & ne7 & e8;

	bdd t08 = ne1 & e2 & e3 & ne4 & ne5 & ne6 & ne7 & ne8;
	bdd t09 = ne1 & e2 & ne3 & e4 & ne5 & ne6 & ne7 & ne8;
	bdd t10 = ne1 & e2 & ne3 & ne4 & e5 & ne6 & ne7 & ne8;
	bdd t11 = ne1 & e2 & ne3 & ne4 & ne5 & e6 & ne7 & ne8;
	bdd t12 = ne1 & e2 & ne3 & ne4 & ne5 & ne6 & e7 & ne8;
	bdd t13 = ne1 & e2 & ne3 & ne4 & ne5 & ne6 & ne7 & e8;

	bdd t14 = ne1 & ne2 & e3 & e4 & ne5 & ne6 & ne7 & ne8;
	bdd t15 = ne1 & ne2 & e3 & ne4 & e5 & ne6 & ne7 & ne8;
	bdd t16 = ne1 & ne2 & e3 & ne4 & ne5 & e6 & ne7 & ne8;
	bdd t17 = ne1 & ne2 & e3 & ne4 & ne5 & ne6 & e7 & ne8;
	bdd t18 = ne1 & ne2 & e3 & ne4 & ne5 & ne6 & ne7 & e8;

	bdd t19 = ne1 & ne2 & ne3 & e4 & e5 & ne6 & ne7 & ne8;
	bdd t20 = ne1 & ne2 & ne3 & e4 & ne5 & e6 & ne7 & ne8;
	bdd t21 = ne1 & ne2 & ne3 & e4 & ne5 & ne6 & e7 & ne8;
	bdd t22 = ne1 & ne2 & ne3 & e4 & ne5 & ne6 & ne7 & e8;

	bdd t23 = ne1 & ne2 & ne3 & ne4 & e5 & e6 & ne7 & ne8;
	bdd t24 = ne1 & ne2 & ne3 & ne4 & e5 & ne6 & e7 & ne8;
	bdd t25 = ne1 & ne2 & ne3 & ne4 & e5 & ne6 & ne7 & e8;

	bdd t26 = ne1 & ne2 & ne3 & ne4 & ne5 & e6 & e7 & ne8;
	bdd t27 = ne1 & ne2 & ne3 & ne4 & ne5 & e6 & ne7 & e8;

	bdd t28 = ne1 & ne2 & ne3 & ne4 & ne5 & ne6 & e7 & e8;

	bdd limit = p & (t00 | t01 | t02 | t03 | t04 | t05 | t06 | t07 |
						  t08 | t09 | t10 | t11 | t12 | t13 | t14 | t15 |
						  t16 | t17 | t18 | t19 | t20 | t21 | t22 | t23 |
						  t24 | t25 | t26 | t27 | t28 );
	f = f | limit;
	//	bdd f = !p & !e1 & !e2 & !e3 & !e4 & !e5 & !e6 & !e7 & !e8;
	//int notin[] = {9,10,11,12,13,14,15,16,17};
	//bdd fixvars = bdd_makeset(notin,9);
	//f = bdd_restrict(f,fixvars);



//	bdd notin = bdd_nithvar(9) & bdd_nithvar(10) & bdd_nithvar(11) & bdd_nithvar(12) & bdd_nithvar(13) & bdd_nithvar(14) & bdd_nithvar(15) & bdd_nithvar(16) & bdd_nithvar(17);
//	f = bdd_apply(f,notin,bddop_and);
//	f = f;
	//bdd_fnprintdot((char *)"constraint.dot",f);
	//printf("Constraint has %d solutions\n",bdd_satcount(f & bddfalse));
	//bdd_allsat(f, *satPrinter);
	return f;
}

