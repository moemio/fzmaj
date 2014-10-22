#include "stdio.h"
#include "string.h"
#include "error.h"
#include "syanten.h"
#include <cmath>

using namespace FZMAJ_NS;

Syanten::Syanten(FZMAJ *maj) : Pointers(maj)
{
	init();
}

Syanten::~Syanten()
{

}

void Syanten::init()
{
	int i;
	for (i=0;i<34;i++)
		c[i]=0;
	n_mentsu	 = 0;
	n_tatsu      = 0;
	n_toitsu     = 0;
	n_jidahai    = 0;
	f_n4         = 0;
	f_koritsu    = 0;
	min_syanten  = 8;
	st_normal    = 8;
	st_7         = 8;
	st_13        = 8;
}

void Syanten::updateResult()
{
	int ret_syanten = 8-n_mentsu*2-n_tatsu-n_toitsu;
	int n_mentsu_kouho = n_mentsu+n_tatsu;
	if (n_toitsu) {
		n_mentsu_kouho+=n_toitsu-1;
	} else if (f_n4 && f_koritsu) {
		if ((f_n4|f_koritsu)==f_n4) ++ret_syanten;
	}
	if (n_mentsu_kouho>4) ret_syanten+=(n_mentsu_kouho-4);
	if (ret_syanten!=-1 && ret_syanten<n_jidahai) ret_syanten=n_jidahai;
	if (ret_syanten<st_normal){
		st_normal = ret_syanten;
	}
}


void Syanten::i_anko(int k)    { c[k]-=3; ++n_mentsu;}
void Syanten::d_anko(int k)    { c[k]+=3; --n_mentsu;}
void Syanten::i_toitsu(int k)  { c[k]-=2; ++n_toitsu;}
void Syanten::d_toitsu(int k)  { c[k]+=2; --n_toitsu;}
void Syanten::i_syuntsu(int k) { --c[k]; --c[k+1]; --c[k+2]; ++n_mentsu;}
void Syanten::d_syuntsu(int k) { ++c[k]; ++c[k+1]; ++c[k+2]; --n_mentsu;}
void Syanten::i_tatsu_r(int k) { --c[k]; --c[k+1]; ++n_tatsu;}
void Syanten::d_tatsu_r(int k) { ++c[k]; ++c[k+1]; --n_tatsu;}
void Syanten::i_tatsu_k(int k) { --c[k]; --c[k+2]; ++n_tatsu;}
void Syanten::d_tatsu_k(int k) { ++c[k]; ++c[k+2]; --n_tatsu;}
void Syanten::i_koritsu(int k) { --c[k]; f_koritsu|=(1<<k);}
void Syanten::d_koritsu(int k) { ++c[k]; f_koritsu&=~(1<<k);}

int Syanten::count34()
{
	return c[ 0]+c[ 1]+c[ 2]+c[ 3]+c[ 4]+c[ 5]+c[ 6]+c[ 7]+c[ 8]+
	       c[ 9]+c[10]+c[11]+c[12]+c[13]+c[14]+c[15]+c[16]+c[17]+
		   c[18]+c[19]+c[20]+c[21]+c[22]+c[23]+c[24]+c[25]+c[26]+
		   c[27]+c[28]+c[29]+c[30]+c[31]+c[32]+c[33];
}

void Syanten::scan713()
{
	int n13 = (c[ 0]>=2)+(c[ 8]>=2)+
	          (c[ 9]>=2)+(c[17]>=2)+
			  (c[18]>=2)+(c[26]>=2)+
			  (c[27]>=2)+(c[28]>=2)+(c[29]>=2)+(c[30]>=2)+(c[31]>=2)+(c[32]>=2)+(c[33]>=2);
	int m13 = (c[ 0]!=0)+(c[ 8]!=0)+
	          (c[ 9]!=0)+(c[17]!=0)+
			  (c[18]!=0)+(c[26]!=0)+
			  (c[27]!=0)+(c[28]!=0)+(c[29]!=0)+(c[30]!=0)+(c[31]!=0)+(c[32]!=0)+(c[33]!=0);
	int n7 = n13+
			  (c[ 1]>=2)+(c[ 2]>=2)+(c[ 3]>=2)+(c[ 4]>=2)+(c[ 5]>=2)+(c[ 6]>=2)+(c[ 7]>=2)+
			  (c[10]>=2)+(c[11]>=2)+(c[12]>=2)+(c[13]>=2)+(c[14]>=2)+(c[15]>=2)+(c[16]>=2)+
			  (c[19]>=2)+(c[20]>=2)+(c[21]>=2)+(c[22]>=2)+(c[23]>=2)+(c[24]>=2)+(c[25]>=2);
	int m7 = m13+
			  (c[ 1]!=0)+(c[ 2]!=0)+(c[ 3]!=0)+(c[ 4]!=0)+(c[ 5]!=0)+(c[ 6]!=0)+(c[ 7]!=0)+
			  (c[10]!=0)+(c[11]!=0)+(c[12]!=0)+(c[13]!=0)+(c[14]!=0)+(c[15]!=0)+(c[16]!=0)+
			  (c[19]!=0)+(c[20]!=0)+(c[21]!=0)+(c[22]!=0)+(c[23]!=0)+(c[24]!=0)+(c[25]!=0);
	
	st_7 = 6-n7+(m7<7?7-m7:0);
	st_13 = 13-m13-(n13?1:0);
}

void Syanten::removeJihai(int nc)
{
	int j_n4=0;
	int j_koritsu=0;
	int i;
	for(i=27;i<34;++i) switch(c[i]) {
		case 4: {++n_mentsu; j_n4|=(1<<(i-18)); j_koritsu|=(1<<(i-18)); ++n_jidahai; break;}
		case 3: {++n_mentsu; break;}
		case 2: {++n_toitsu; break;}
		case 1: {j_koritsu|=(1<<(i-18));break;}
		}
	if (n_jidahai && (nc%3)==2) --n_jidahai;

	if (j_koritsu) {
		f_koritsu|=(1<<27);
		if ((j_n4|j_koritsu)==j_n4) f_n4|=(1<<27);
	}
}

void Syanten::scanNormal(int init_mentsu)
{
	f_n4|=
		((c[ 0]==4)<< 0)|((c[ 1]==4)<< 1)|((c[ 2]==4)<< 2)|
		((c[ 3]==4)<< 3)|((c[ 4]==4)<< 4)|((c[ 5]==4)<< 5)|
		((c[ 6]==4)<< 6)|((c[ 7]==4)<< 7)|((c[ 8]==4)<< 8)|
		((c[ 9]==4)<< 9)|((c[10]==4)<<10)|((c[11]==4)<<11)|
		((c[12]==4)<<12)|((c[13]==4)<<13)|((c[14]==4)<<14)|
		((c[15]==4)<<15)|((c[16]==4)<<16)|((c[17]==4)<<17)|
		((c[18]==4)<<18)|((c[19]==4)<<19)|((c[20]==4)<<20)|
		((c[21]==4)<<21)|((c[22]==4)<<22)|((c[23]==4)<<23)|
		((c[24]==4)<<24)|((c[25]==4)<<25)|((c[26]==4)<<26);
	n_mentsu += init_mentsu;
	Run(0);
}

void Syanten::Run(int depth)
{
	++n_eval;
	if (st_normal==-1) return;
	for(;depth<27&&!c[depth];++depth);
	if (depth==27) return updateResult();
	
	int i=depth;
	if (i>8) i-=9;
	if (i>8) i-=9;
	switch (c[depth]) {
	case 4: {
		i_anko(depth);
		if (i<7&&c[depth+2]) {
			if (c[depth+1]) {i_syuntsu(depth); Run(depth+1); d_syuntsu(depth);}
			i_tatsu_k(depth); Run(depth+1); d_tatsu_k(depth);
		}
		if (i<8 && c[depth+1]) { i_tatsu_r(depth); Run(depth+1); d_tatsu_r(depth);}
		i_koritsu(depth); Run(depth+1); d_koritsu(depth);
		d_anko(depth);

		i_toitsu(depth);
		if (i<7&&c[depth+2]) {
			if (c[depth+1]) {i_syuntsu(depth); Run(depth); d_syuntsu(depth);}
			i_tatsu_k(depth); Run(depth+1); d_tatsu_k(depth);
		}
		if (i<8&&c[depth+1]) {i_tatsu_r(depth); Run(depth+1); d_tatsu_r(depth);}
		d_toitsu(depth);
		break;}
	case 3: {
		i_anko(depth); Run(depth+1); d_anko(depth);

		i_toitsu(depth);
		if (i<7&&c[depth+1]&&c[depth+2]) {
			i_syuntsu(depth); Run(depth+1); d_syuntsu(depth);
		} else {
			if (i<7&&c[depth+2]) { i_tatsu_k(depth); Run(depth+1); d_tatsu_k(depth);}
			if (i<8&&c[depth+1]) { i_tatsu_r(depth); Run(depth+1); d_tatsu_r(depth);}
		}
		d_toitsu(depth);
		if (i<7&&c[depth+2]>=2&&c[depth+1]>=2) { i_syuntsu(depth);
			i_syuntsu(depth); Run(depth); d_syuntsu(depth); d_syuntsu(depth);}
		break;}
	case 2: {
		i_toitsu(depth); Run(depth+1); d_toitsu(depth);
		if (i<7&&c[depth+2]&&c[depth+1]) {i_syuntsu(depth); Run(depth); d_syuntsu(depth);}
		break; }
	case 1: {
		if (i<6&&c[depth+1]==1&&c[depth+2]&&c[depth+3]!=4) {
			i_syuntsu(depth); Run(depth+2); d_syuntsu(depth);
		} else {
		//	i_koritsu(depth); Run(depth+1); d_koritsu(depth);
			i_koritsu(depth); Run(depth+1); d_koritsu(depth);

			if (i<7&&c[depth+2]) {
				if (c[depth+1]) {i_syuntsu(depth); Run(depth+1); d_syuntsu(depth);}
				i_tatsu_k(depth); Run(depth+1); d_tatsu_k(depth);
			}
			if (i<8&&c[depth+1]) {i_tatsu_r(depth); Run(depth+1); d_tatsu_r(depth);}
		}
		break;}
	}
}

int Syanten::calcSyanten(bool isSkip713)
{
	int nc = count34();
	if (nc>14) return -2;
	if (!isSkip713 && nc>=13) scan713();
	removeJihai(nc);
	double imt = ((14-(float)nc)/3);
	int init_mentsu=(int)floor(imt);
	scanNormal(init_mentsu);

	min_syanten = MIN(MIN(st_7,st_13),st_normal);

	printf("st_normal : %d\n",st_normal);
	printf("st_7 : %d\n",st_7);
	printf("st_13 : %d\n",st_13);
	printf("min_syanten : %d\n",min_syanten);

}

int Syanten::calcSyantenAll(int tehai[])
{
	c[0] = *tehai;
	calcSyanten(false);
	return min_syanten;
}

int Syanten::calcSyantenNo713(int tehai[])
{
	c[0] = *tehai;
	calcSyanten(true);
	return min_syanten;
}
