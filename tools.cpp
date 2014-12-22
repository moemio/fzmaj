#include "tools.h"
#include "stdlib.h"
#include "string.h"
#include "error.h"

using namespace FZMAJ_NS;

Tools::Tools(FZMAJ *maj) : Pointers(maj) {}

Tools::~Tools(){}

int Tools::Str2pai(char *spai, int c[])
{
	int i, e=-1, l, t;
	int last;
	int h[3]={0,0,0};
	for (i=0;i<34;i++)
		c[i]=0;
	l=strlen(spai);
	for (i=l-1;i>=0;i--) {
		if(spai[i]=='m') e=0;
		else if (spai[i]=='p') e=1;
		else if (spai[i]=='s') e=2;
		else if (spai[i]=='z') e=3;
		else if (spai[i]>='0' && spai[i]<='9') {
			if (e==-1) error->all(FLERR, "Illegal string for pai");
			t=spai[i]-'0';
			if (t==0) {
				if (e>=3) error->all(FLERR, "Illegal string for pai");
				if (h[e]==0) {
					h[e]++; t=5;
				}
				else error->all(FLERR, "Illegal string for pai");
			}
			t+=e*9-1;
			if (t>33) error->all(FLERR,"Illegal string for pai");
			if (c[t]<4) c[t]++; else error->all(FLERR,"Illegal string for pai");
			if (i==l-2) last=t;
		}
		else error->all(FLERR, "Illegal string for pai");
	}
	return last;
}

int Tools::CountPai(int c[])
{
	return c[ 0]+c[ 1]+c[ 2]+c[ 3]+c[ 4]+c[ 5]+c[ 6]+c[ 7]+c[ 8]+c[ 9]+
	       c[10]+c[11]+c[12]+c[13]+c[14]+c[15]+c[16]+c[17]+c[18]+c[19]+
	       c[20]+c[21]+c[22]+c[23]+c[24]+c[25]+c[26]+c[27]+c[28]+c[29]+
	       c[30]+c[31]+c[32]+c[33];
}

void Tools::randPerm(int n, int perm[])
{
	int i,j,t;
	for(i=0;i<n;i++)
		perm[i]=i;
	for(i=0;i<n;i++) {
		j = rand()%(n-i)+i;
		t = perm[j];
		perm[j] = perm[i];
		perm[i] = t;
	}
}

int Tools::check_aka(int n)
{
	if (n==16) return 1;
	if (n==52) return 2;
	if (n==88) return 3;
	return 0;
}

string Tools::Pai2str(int cpai, int aka)
{
	string sth = "";
	char tp;
	if (cpai>=9) {cpai -= 9;tp='p';}
	else tp='m';
	if (cpai>=9) {cpai -= 9;tp='s';}
	if (cpai>=9) {cpai -= 9;tp='z';}
	if (aka) sth += '0';
	else sth += ('1'+cpai);
	sth += tp;
	return sth;
}
