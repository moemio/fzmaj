#include "stdio.h"
#include "error.h"
#include "tools.h"
#include "agari.h"

using namespace FZMAJ_NS;

Agari::Agari(FZMAJ *maj) : Pointers(maj)
{}

Agari::~Agari() {}

void Agari::tokuHandan()
{
	int i,j;
	PATTERN part;
	for(i=0;i<34;++i) {
		part.c[i]=c[i];
		part.syuntsu[i]=0;
		part.kotsu[i]=0;
		part.naki_syuntsu[i]=0;
		part.naki_kotsu[i]=0;
	}
	part.n_syuntsu=0;
	part.n_kotsu=0;
	part.n_syuntsu=0;
	part.n_kotsu=0;
	part.n_naki=0;
	part.atama=-1;
	part.isYakuman = false;
	part.yakuman_baisu = 0;

	// Kokushi musou
	if (c[0]*c[8]*c[9]*c[17]*c[18]*c[26]*c[27]*c[28]*c[29]*c[30]*c[31]*c[32]*c[33]==2) {
		part.isChiitoi = false;
		part.isKokushi = true;
		part.isYakuman = true;
		for(i=0;i<34;++i)
			if(c[i]==2) j=i;
		part.atama=j;

		pattern.push_back(part);
	}

	// Chiitoi
	if (((c[ 0]==2)+(c[ 1]==2)+(c[ 2]==2)+(c[ 3]==2)+(c[ 4]==2)+(c[ 5]==2)+(c[ 6]==2)+(c[ 7]==2)+(c[ 8]==2)+
		 (c[ 9]==2)+(c[10]==2)+(c[11]==2)+(c[12]==2)+(c[13]==2)+(c[14]==2)+(c[15]==2)+(c[16]==2)+(c[17]==2)+
		 (c[18]==2)+(c[19]==2)+(c[20]==2)+(c[21]==2)+(c[22]==2)+(c[23]==2)+(c[24]==2)+(c[25]==2)+(c[26]==2)+
		 (c[27]==2)+(c[28]==2)+(c[29]==2)+(c[30]==2)+(c[31]==2)+(c[32]==2)+(c[33]==2))==7) {
		
		part.isChiitoi = true;
		part.isKokushi = false;
		for(i=0;i<34;++i)
			part.toitsu[i]=(c[i]==2);

		pattern.push_back(part);
	}

	delete &part;
	// Otherwise

	removeJihai();
	Run(0);
}


void Agari::checkAgari(Bakyou *bakyou)
{
	int i;
	for(i=0;i<34;++i) {
		c[i]=bakyou->tehai[i];
		kotsu[i]=0;
		syuntsu[i]=0;
	}
	pattern.clear();
	for(i=0;i<46;++i)
		yaku[i]=0;
	bak = bakyou;

	tokuHandan();
}


void Agari::removeJihai()
{
	int i;
	for(i=27;i<34;++i) {
		if (c[i]==2) i_atama(i);
		if (c[i]==3) i_kotsu(i);
	}	
}

void Agari::i_kotsu(int k)
	{c[k]-=3; ++n_mentsu; ++kotsu[k]; ++n_kotsu;}

void Agari::d_kotsu(int k)
	{c[k]+=3; --n_mentsu; --kotsu[k]; --n_kotsu;}

void Agari::i_syuntsu(int k)
	{--c[k];--c[k+1];--c[k+2]; ++n_mentsu; ++n_syuntsu; ++syuntsu[k];}

void Agari::d_syuntsu(int k)
	{++c[k];++c[k+1];++c[k+2]; --n_mentsu; --n_syuntsu; --syuntsu[k];}

void Agari::i_atama(int k)
	{c[k]-=2; atama=k; isatama = 1;}

void Agari::d_atama(int k)
	{c[k]+=2; isatama = 0;}


int Agari::cc2m(int d)
{
	return (c[d+0]<< 0)|(c[d+1]<< 3)|(c[d+2]<< 6)|
	       (c[d+3]<< 9)|(c[d+4]<<12)|(c[d+5]<<15)|
		   (c[d+6]<<18)|(c[d+7]<<21)|(c[d+8]<<24);
}

bool Agari::isMentsu(int m)
{
	int a=(m&7), b=0, c=0;
	if (a==1 || a==4) b=c=1; else if (a==2) b=c=2;
	m>>=3, a=(m&7)-b;if (a<0) return false;b=c, c=0;if (a==1 || a==4) b+=1, c+=1; else if (a==2) b+=2, c+=2;
	m>>=3, a=(m&7)-b;if (a<0) return false;b=c, c=0;if (a==1 || a==4) b+=1, c+=1; else if (a==2) b+=2, c+=2;
	m>>=3, a=(m&7)-b;if (a<0) return false;b=c, c=0;if (a==1 || a==4) b+=1, c+=1; else if (a==2) b+=2, c+=2;
	m>>=3, a=(m&7)-b;if (a<0) return false;b=c, c=0;if (a==1 || a==4) b+=1, c+=1; else if (a==2) b+=2, c+=2;
	m>>=3, a=(m&7)-b;if (a<0) return false;b=c, c=0;if (a==1 || a==4) b+=1, c+=1; else if (a==2) b+=2, c+=2;
	m>>=3, a=(m&7)-b;if (a<0) return false;b=c, c=0;if (a==1 || a==4) b+=1, c+=1; else if (a==2) b+=2, c+=2;
	m>>=3, a=(m&7)-b;if (a!=0 && a!=3) return false;
	m>>=3, a=(m&7)-c;
	return a==0 || a==3;
}

bool Agari::isAtamaMentsu(int nn, int m)
{
	if (nn==0){
		if ((m&(7<< 6))>=(2<< 6) && isMentsu(m-(2<< 6))) return true;
		if ((m&(7<<15))>=(2<<15) && isMentsu(m-(2<<15))) return true;
		if ((m&(7<<24))>=(2<<24) && isMentsu(m-(2<<24))) return true;
	}else if (nn==1){
		if ((m&(7<< 3))>=(2<< 3) && isMentsu(m-(2<< 3))) return true;
		if ((m&(7<<12))>=(2<<12) && isMentsu(m-(2<<12))) return true;
		if ((m&(7<<21))>=(2<<21) && isMentsu(m-(2<<21))) return true;
	}else if (nn==2){
		if ((m&(7<< 0))>=(2<< 0) && isMentsu(m-(2<< 0))) return true;
		if ((m&(7<< 9))>=(2<< 9) && isMentsu(m-(2<< 9))) return true;
		if ((m&(7<<18))>=(2<<18) && isMentsu(m-(2<<18))) return true;
	}
	return false;
}

void Agari::Run(int depth)
{
	for (; depth<27&&!c[depth];++depth);
	updateResult();
	int i=depth;
	if (i>8) i-=9;
	if (i>8) i-=9;
	switch( c[depth] ) {
	case 4: {
		i_kotsu(depth);
		if (i<7&&c[depth+1]&&c[depth+2]) {
			i_syuntsu(depth); Run(depth+1); d_syuntsu(depth);
		}
		d_kotsu(depth);
		if (!isatama) {
			i_atama(depth);
			if (i<7&&c[depth+1]&&c[depth+2]) {
				i_syuntsu(depth); Run(depth); d_syuntsu(depth);  //marker : run(depth+1)
			}
			d_atama(depth);
		}
		break;}
	case 3:{
		i_kotsu(depth); Run(depth+1); d_kotsu(depth);
		if (!isatama) {
			i_atama(depth);
			if (i<7&&c[depth+1]&&c[depth+2]) {
				i_syuntsu(depth); Run(depth+1); d_syuntsu(depth);  }
			d_atama(depth);
		}
		if (i<7&&c[depth+1]>=2&&c[depth+2]>=2) {
			i_syuntsu(depth); Run(depth); d_syuntsu(depth); } //marker : run twice
		break;}
	case 2:{
		if (!isatama) {
			i_atama(depth); Run(depth+1); d_atama(depth); }
		if (i<7&&c[depth+1]&&c[depth+2]) {
			i_syuntsu(depth); Run(depth); d_syuntsu(depth); } //marker : run twice
		break;}	
	case 1:{
		if (i<7&&c[depth+1]&&c[depth+2]) {
			i_syuntsu(depth); Run(depth+1); d_syuntsu(depth); } 
	break;}
	}
}

void Agari::updateResult()
{
	int i,j;
	PATTERN part;
	part.isYakuman=false;
	part.isChiitoi=false;
	part.isKokushi=false;
	part.yakuman_baisu=0;
	if ((n_mentsu + bak->n_naki[0])==4 && isatama  ) {
		part.n_kotsu=n_kotsu;
		part.n_syuntsu=n_syuntsu;
		part.n_kotsu=bak->n_naki_kotsu[0];
		part.n_syuntsu = bak->n_naki_syuntsu[0];
		for(i=0;i<34;++i) {
			part.c[i]=c[i];
			part.kotsu[i]=kotsu[i];
			part.syuntsu[i]=syuntsu[i];
		    part.naki_kotsu[i]=bak->naki_kotsu[0][i];
		    part.naki_syuntsu[i]=bak->naki_syuntsu[0][i];
		    part.naki_kotsu[i] += bak->naki_kan[0][i];
		    part.kotsu[i]      += bak->naki_ankan[0][i];
		}
		part.atama=atama;
		pattern.push_back(part);
	}
	delete &part;
}
