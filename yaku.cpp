#include "yaku.h"
#include "defs.h"
#include "math.h"
#include "error.h"

using namespace FZMAJ_NS;

Yaku::Yaku(FZMAJ * maj) : Pointers (maj) {

	yakufan[YAKU_MENTSUMO]       =  1;
	yakufan[YAKU_RIICHI]         =  1;	
	yakufan[YAKU_TYANKAN]        =  1;
	yakufan[YAKU_RINSYAN]        =  1;
	yakufan[YAKU_HAITEI]         =  1;
	yakufan[YAKU_HOUTEI]         =  1;
	yakufan[YAKU_YAKUHAI_BAFUU]  =  1;
	yakufan[YAKU_YAKUHAI_JIFUU]  =  1;
	yakufan[YAKU_YAKUHAI_SANGEN] =  1;
	yakufan[YAKU_TANYAO]         =  1;
	yakufan[YAKU_IPPEI]          =  1;
	yakufan[YAKU_PINFU]          =  1;

	yakufan[YAKU_HONCHAN]        =  2;
	yakufan[YAKU_IKKITSUKAN]     =  2;
	yakufan[YAKU_SANSHOKU]       =  2;
	yakufan[YAKU_WRIICHI]        =  2;
	yakufan[YAKU_TONKO]          =  2;
	yakufan[YAKU_SANKANTSU]      =  2;
	yakufan[YAKU_TOITOI]         =  2;
	yakufan[YAKU_SANANKO]        =  2;
	yakufan[YAKU_SYAOSANGEN]     =  2;
	yakufan[YAKU_HONROU]         =  2;
	yakufan[YAKU_CHIITOI]        =  2;
	
	yakufan[YAKU_JONCHAN]        =  3;
	yakufan[YAKU_HONIISOU]       =  3;
	yakufan[YAKU_RYANPEI]        =  3;
	
	yakufan[YAKU_CHINIISOU]      =  6;
	
	yakufan[YAKU_IPPATSU]        =  1;
	yakufan[YAKU_DORA]			 =  1;
	yakufan[YAKU_URA]            =  1;
	yakufan[YAKU_AKA]            =  1;
	
	yakufan[YAKU_TENHOU]         = 13;
	yakufan[YAKU_CHIHOU]         = 13;
	yakufan[YAKU_DASANGEN]       = 13;
	yakufan[YAKU_SUANKO]         = 13;
	yakufan[YAKU_SUANKO_TANKI]   = 13;
	yakufan[YAKU_TSUIISOU]       = 13;
	yakufan[YAKU_RYUIISOU]       = 13;
	yakufan[YAKU_CHINROU]        = 13;
	yakufan[YAKU_KOKUSHI]        = 13;
	yakufan[YAKU_KOKUSHI_13]     = 13;
	yakufan[YAKU_DASUSHI]        = 13;
	yakufan[YAKU_SYAOSUSHI]      = 13;
	yakufan[YAKU_SUKANTSU]       = 13;
	yakufan[YAKU_CHUUREN]        = 13;
	yakufan[YAKU_JONCHUUREN]     = 13;


}

Yaku::~Yaku() {}

int Yaku::countYaku(Bakyou *bk, PATTERN &part)
{
	pattern = &part;
	bak = bk;
	int i;	
	for(i=0;i<46;i++)
		yakus[i]=0;
	countFan();
	return 0; 
}

int Yaku::IsMenTsumo() 
{	return (!bak->n_naki[0]) && bak->dacya==0 && (bak->act==ACT_AGARI_TSUMO || bak->act==ACT_KAN_SELF); }

int Yaku::IsRiichi()
{	return bak->riichi[0]>1; }

int Yaku::IsTyankan()
{	return bak->act==ACT_KAN; }

int Yaku::IsRinsyan()
{	return bak->act==ACT_KAN_SELF; }

int Yaku::IsHaitei()
{	return (bak->pai_ptr==bak->dead_ptr) && (bak->act==ACT_AGARI_TSUMO) ; }

int Yaku::IsHoutei()
{	return (bak->pai_ptr==bak->dead_ptr) && (bak->dacya!=0) ; }

int Yaku::IsYakuhaiBafuu()
{	return pattern->kotsu[bak->bafuu] || pattern->naki_kotsu[bak->bafuu] ; }

int Yaku::IsYakuhaiJifuu()
{	return pattern->kotsu[bak->jifuu] || pattern->naki_kotsu[bak->jifuu] ; }

int Yaku::IsYakuhaiSangen()
{	
	int n=0;
	if (pattern->kotsu[31] || pattern->naki_kotsu[31]) ++n;
	if (pattern->kotsu[32] || pattern->naki_kotsu[32]) ++n;
	if (pattern->kotsu[33] || pattern->naki_kotsu[33]) ++n;
	return n;
}

int Yaku::IsTanyao()
{
	int* c = pattern->c;
	return !(c[ 0] + c[ 8] + c[ 9] + c[17] + c[18] + c[26] +
			 c[27] + c[28] + c[29] + c[30] + c[31] + c[32] + c[33]);
}

int Yaku::CheckPeiko()
{
	int est=0,i;
	if (pattern->n_syuntsu>=2)
		for(i=0;i<34;i++)
			est += (pattern->syuntsu[i]>=2);
	return est;
}

int Yaku::IsIppei()
{	return (CheckPeiko()==1) && (!bak->n_naki[0]); }

int Yaku::IsPinfu()
{
	if (bak->n_naki[0]) return 0;
	if (pattern->n_syuntsu!=4) return 0;
	int i = pattern->atama;
	if (i==bak->bafuu || i==bak->jifuu || (i>30)) return 0;
	for(i=0;i<34;i++)
		if(pattern->syuntsu[i])
			if((bak->syanpai==i && i%9!=6) || (bak->syanpai==i+2 && i%9!=0)) return 1;
	return 0;
}
int Yaku::Is19z(int k)
{	return (k==0)+(k== 8)+(k== 9)+(k==17)+(k==18)+(k==26)+(k==27)
	             +(k==28)+(k==29)+(k==30)+(k==31)+(k==32)+(k==33); }

int Yaku::Is19(int k)
{	return (k==0)+(k==8)+(k==9)+(k==17)+(k==18)+(k==26); }

int Yaku::IsChantai()
{
	if (!Is19z(pattern->atama)) return 0;
	if (pattern->n_kotsu+pattern->n_syuntsu!=4) return 0;
	int i;
	for(i=0;i<34;i++) {
		if (pattern->kotsu[i])
			if (!Is19z(i)) return 0;
		if (pattern->naki_kotsu[i])
			if (!Is19z(i)) return 0;
		if (pattern->syuntsu[i]) {
			if ((i!= 0) && (i!= 9) && (i!=18) && (i!= 6) && (i!=15) && (i!=24)) return 0;
			}
		if (pattern->naki_syuntsu[i])
			if ((i!= 0) && (i!= 9) && (i!=18) && (i!= 6) && (i!=15) && (i!=24)) return 0;
		}
	int *c = pattern->c;
	int nz=c[27]+c[28]+c[29]+c[30]+c[31]+c[32]+c[33];
	return nz>0 && nz<14 ; 
}

int Yaku::IsIkki()
{
	return ((pattern->syuntsu[ 0] || pattern->naki_syuntsu[ 0]) && 
	        (pattern->syuntsu[ 3] || pattern->naki_syuntsu[ 3]) &&
		    (pattern->syuntsu[ 6] || pattern->naki_syuntsu[ 6]) )
	   ||  ((pattern->syuntsu[ 9] || pattern->naki_syuntsu[ 9]) &&
	        (pattern->syuntsu[12] || pattern->naki_syuntsu[12]) &&
 		    (pattern->syuntsu[15] || pattern->naki_syuntsu[15]) )
	   ||  ((pattern->syuntsu[18] || pattern->naki_syuntsu[18]) &&
	        (pattern->syuntsu[21] || pattern->naki_syuntsu[21]) &&
 		    (pattern->syuntsu[24] || pattern->naki_syuntsu[24]) ) ;
}

int Yaku::IsSanshoku()
{
	int i;
	for(i=0;i<7;i++)
		if ( (pattern->syuntsu[i]    || pattern->naki_syuntsu[i])   &&
		     (pattern->syuntsu[i+9]  || pattern->naki_syuntsu[i+9]) &&
			 (pattern->syuntsu[i+18] || pattern->naki_syuntsu[i+18]) )
			return 1;
	return 0;
}

int Yaku::IsWRiichi()
{	return bak->riichi[0]==1; }

int Yaku::IsTonko()
{
	int i;
	for(i=0;i<9;i++)
		if ( (pattern->kotsu[i]    || pattern->naki_kotsu[i])   &&
			 (pattern->kotsu[i+9]  || pattern->naki_kotsu[i+9]) &&
			 (pattern->kotsu[i+18] || pattern->naki_kotsu[i+18]) )
			return 1;
	return 0;
}

int Yaku::IsSankantsu()
{	return bak->n_naki_kan[0]+bak->n_naki_ankan[0]==3; }

int Yaku::IsToitoi()
{

return (pattern->n_kotsu)==4; }

int Yaku::IsSananko()
{	
return (pattern->n_kotsu==3 && bak->act==ACT_AGARI_TSUMO) ||
		   (pattern->n_kotsu==3 && bak->dacya!=0 && !pattern->kotsu[bak->syanpai]) ||
		   (pattern->n_kotsu==3 && bak->dacya!=0 && pattern->kotsu[bak->syanpai] && 
		   pattern->c[bak->syanpai]==4) ||
		   (pattern->n_kotsu==4 && bak->dacya!=0 && pattern->kotsu[bak->syanpai]); }

int Yaku::IsSyaosangen()
{	return (pattern->atama==31 && pattern->kotsu[32] && pattern->kotsu[33]) ||
           (pattern->atama==32 && pattern->kotsu[31] && pattern->kotsu[33]) ||
		   (pattern->atama==33 && pattern->kotsu[31] && pattern->kotsu[32]) ;  }

int Yaku::IsHonrou()
{
	int est=0,i;
	int *c=pattern->c;
	for(i=1;i<8;i++)
		est+=(c[i]+c[i+9]+c[i+18]);
	return (!est) && (c[27]+c[28]+c[29]+c[30]+c[31]+c[32]+c[33]);
}

int Yaku::IsChiitoi()
{
	return (pattern->n_syuntsu==0 && pattern->n_kotsu==0 && pattern->n_naki==0 && pattern->isChiitoi);
}

int Yaku::IsJonchan()
{
	if (!Is19(pattern->atama)) return 0;
	if (pattern->n_kotsu+pattern->n_syuntsu!=4) return 0;
	int i;
	for(i=0;i<34;i++) {
		if (pattern->kotsu[i])
			if (!Is19(i)) return 0;
		if (pattern->naki_kotsu[i])
			if (!Is19(i)) return 0;
		if (pattern->syuntsu[i])
			if ((i!= 0) && (i!= 9) && (i!=18) && (i!= 6) && (i!=15) && (i!=24)) return 0;
		if (pattern->naki_syuntsu[i])
			if ((i!= 0) && (i!= 9) && (i!=18) && (i!= 6) && (i!=15) && (i!=24)) return 0;
		}
	return 1;
}

int Yaku::IsHoniisou()
{
	int *c = pattern->c;
	int nm = c[ 0]+c[ 1]+c[ 2]+c[ 3]+c[ 4]+c[ 5]+c[ 6]+c[ 7]+c[ 8];
	int np = c[ 9]+c[10]+c[11]+c[12]+c[13]+c[14]+c[15]+c[16]+c[17];
	int ns = c[18]+c[19]+c[20]+c[21]+c[22]+c[23]+c[24]+c[25]+c[26];
	int nz = c[27]+c[28]+c[29]+c[30]+c[31]+c[32]+c[33];

	return nz && ((!nm && !np) || (!nm && !ns) || (!np && !ns)) && (nm || np || ns);
}

int Yaku::IsRyanpei()
{	return (this->CheckPeiko()==2) && (!bak->n_naki[0]); }

int Yaku::IsChiniisou()
{
	int *c = pattern->c;
	int nm = c[ 0]+c[ 1]+c[ 2]+c[ 3]+c[ 4]+c[ 5]+c[ 6]+c[ 7]+c[ 8];
	int np = c[ 9]+c[10]+c[11]+c[12]+c[13]+c[14]+c[15]+c[16]+c[17];
	int ns = c[18]+c[19]+c[20]+c[21]+c[22]+c[23]+c[24]+c[25]+c[26];

	return (nm==14) || (np==14) || (ns==14);	
}	
	
int Yaku::IsIppatsu()
{	if(!bak->riichi[0]) return 0;
	return (bak->dacya!=0 && bak->jun[0]==bak->riichi[0]) ||
		   (bak->act==ACT_AGARI_TSUMO && bak->jun[0]==bak->riichi[0]+1); }

int Yaku::IsDora()
{	
	int i,est=0;
	for (i=0;i<bak->n_dora;i++)
		est+=pattern->c[bak->dora[i]];
	return est;
}

int Yaku::IsUra()
{
	int i,est=0;
	if (!bak->riichi[0]) return 0;
	for (i=0;i<bak->n_dora;i++)
		est+=pattern->c[bak->ura[i]];
	return est;
}

int Yaku::IsAka()
{
	int i,est=0;
	for(i=0;i<3;i++) {
		est += bak->aka_tehai[i];
		est += bak->aka_naki[0][i];
	}
	return est;
}

// YAKUMAN
int Yaku::IsTenhou()
{	return bak->pai_ptr==52 && bak->act==ACT_AGARI_TSUMO; }

int Yaku::IsChihou()
{	return bak->jun[0]==1 && bak->pai_ptr>52 && bak->act==ACT_AGARI_TSUMO;}

int Yaku::IsDasangen()
{
	return (pattern->kotsu[31] || pattern->naki_kotsu[31]) &&
		   (pattern->kotsu[32] || pattern->naki_kotsu[32]) &&
		   (pattern->kotsu[33] || pattern->naki_kotsu[33]) ;
}

int Yaku::IsSuanko()
{	return pattern->n_kotsu==4 && bak->n_naki[0] && bak->act==ACT_AGARI_TSUMO && pattern->atama!=bak->syanpai;}

int Yaku::IsSuanko_tanki()
{	return pattern->n_kotsu==4 && bak->n_naki[0]==0 && pattern->atama==bak->syanpai; }

int Yaku::IsTsuiisou()
{	
	int *c = pattern->c;
	return (c[27]+c[28]+c[29]+c[30]+c[31]+c[32]+c[33])==14;
}

int Yaku::IsRyuiisou()
{
	int *c = pattern->c;
	return (c[ 0]+c[ 1]+c[ 2]+c[ 3]+c[ 4]+c[ 5]+c[ 6]+c[ 7]+c[ 8]+
	        c[ 9]+c[10]+c[11]+c[12]+c[13]+c[14]+c[15]+c[16]+c[17]+
		    c[18]+                  c[22]+      c[24]+      c[26]+
		    c[27]+c[28]+c[29]+c[30]+c[31]+      c[33])==0;
}

int Yaku::IsChinrou()
{
	int *c = pattern->c;
	return (c[ 1]+c[ 2]+c[ 3]+c[ 4]+c[ 5]+c[ 6]+c[ 7]+
	        c[10]+c[11]+c[12]+c[13]+c[14]+c[15]+c[16]+
		    c[19]+c[20]+c[21]+c[22]+c[23]+c[24]+c[25]+
		    c[27]+c[28]+c[29]+c[30]+c[31]+c[32]+c[33])==0;
}

int Yaku::IsKokushi()
{	return ((pattern->isKokushi) && (pattern->atama!=bak->syanpai));}

int Yaku::IsKokushi_13()
{	return ((pattern->isKokushi) && (pattern->atama==bak->syanpai));}

int Yaku::IsDasushi()
{
	return ( (pattern->kotsu[27] || pattern->naki_kotsu[27]) &&
			 (pattern->kotsu[28] || pattern->naki_kotsu[28]) &&
			 (pattern->kotsu[29] || pattern->naki_kotsu[29]) &&
			 (pattern->kotsu[30] || pattern->naki_kotsu[30]) ); }

int Yaku::IsSyaosushi()
{
	int i,est=0;
	for(i=27;i<31;i++) {
		if( (pattern->kotsu[i] || pattern->naki_kotsu[i]) ) est++;
		if (pattern->atama==i) est+=2;
	}
	return est==5;
}

int Yaku::IsSukantsu()
{
	return bak->n_naki_ankan[0]+bak->n_naki_kan[0]==4;
}

int Yaku::IsChuuren()
{
	int i,j,k,est;
	int jon=0;
	int *c = pattern->c;
	for(i=0;i<3;i++){
		est=0;
		for(j=0;j<9;j++) {
			k=j+i*9;
			if(j==0 && c[k]>=3) ++est;
			else if (j==8 && c[k]>=3) ++est;
			else if (j>0 && j<8 && c[k]>=1) ++est;
		}
		if (est==9) {
			j=bak->syanpai;
			k=j-i*9;
			if(k==0 && c[j]==4) jon=2;
			else if (k==8 && c[j]==4) jon=2;
			else if (c[j]==2) jon=2;
			else jon=1;
			break;
		}
	}
	return jon;
}

void Yaku::countFan()
{
	int i;
	// Menchin Tsumo
	if (IsMenTsumo()) yakus[YAKU_MENTSUMO]=yakufan[YAKU_MENTSUMO];
	
	// Riichi
	if (IsRiichi()) yakus[YAKU_RIICHI]=yakufan[YAKU_RIICHI];

	// Tyankan
	if (IsTyankan()) yakus[YAKU_TYANKAN]=yakufan[YAKU_TYANKAN];

	// Rinsyan
	if (IsRinsyan()) yakus[YAKU_RINSYAN]=yakufan[YAKU_RINSYAN];
	
	// Haitei
	if (IsHaitei()) yakus[YAKU_HAITEI]=yakufan[YAKU_HAITEI];

	// Houtei
	if (IsHoutei()) yakus[YAKU_HOUTEI]=yakufan[YAKU_HOUTEI];
	
	// Yakuhai
	if (IsYakuhaiBafuu()) yakus[YAKU_YAKUHAI_BAFUU]=yakufan[YAKU_YAKUHAI_BAFUU];
	if (IsYakuhaiJifuu()) yakus[YAKU_YAKUHAI_JIFUU]=yakufan[YAKU_YAKUHAI_JIFUU];
	yakus[YAKU_YAKUHAI_SANGEN] += this->IsYakuhaiSangen()*yakufan[YAKU_YAKUHAI_SANGEN];	
	
	// Tanyao
	if (IsTanyao()) yakus[YAKU_TANYAO]=yakufan[YAKU_TANYAO];
	
	// Ippeikou
	if (IsIppei()) yakus[YAKU_IPPEI]=yakufan[YAKU_IPPEI];
	
	// Pinfu
	if (IsPinfu()) yakus[YAKU_PINFU]=yakufan[YAKU_PINFU];

	// Hon Chantai
	if (IsChantai()) {
		yakus[YAKU_HONCHAN]=yakufan[YAKU_HONCHAN];
		if (bak->n_naki[0]) --yakus[YAKU_HONCHAN];
	}

	// Ikki Tsukan
	if (IsIkki()) {
		yakus[YAKU_IKKITSUKAN]=yakufan[YAKU_IKKITSUKAN];
		if (bak->n_naki[0]) --yakus[YAKU_IKKITSUKAN];
	}

	// Sanshoku Tonsyun
	if (IsSanshoku()) {
		yakus[YAKU_SANSHOKU]=yakufan[YAKU_SANSHOKU];
		if (bak->n_naki[0]) --yakus[YAKU_SANSHOKU];
	}

	// WRiichi
	if (IsWRiichi()) yakus[YAKU_WRIICHI]=yakufan[YAKU_WRIICHI];

	// Sanshoku Tonkou
	if (IsTonko()) yakus[YAKU_TONKO]=yakufan[YAKU_TONKO];

	// San Kantsu
	if (IsSankantsu()) yakus[YAKU_SANKANTSU]=yakufan[YAKU_SANKANTSU];

	// Toitoi
	if (IsToitoi()) yakus[YAKU_TOITOI]=yakufan[YAKU_TOITOI];

	// San Anko
	if (IsSananko()) yakus[YAKU_SANANKO]=yakufan[YAKU_SANANKO];

	// Syao Sengen
	if (IsSyaosangen()) yakus[YAKU_SYAOSANGEN]=yakufan[YAKU_SYAOSANGEN];

	// Hon Routou
	if (IsHonrou()) yakus[YAKU_HONROU]=yakufan[YAKU_HONROU];
	
	// Chii Toitsu
	if (IsChiitoi()) yakus[YAKU_CHIITOI]=yakufan[YAKU_CHIITOI];
	
	// Ippatsu
	if (IsIppatsu()) yakus[YAKU_IPPATSU]=yakufan[YAKU_IPPATSU];

	// Jon Chantai
	if (IsJonchan()) {
		yakus[YAKU_JONCHAN]=yakufan[YAKU_JONCHAN];
		if (bak->n_naki[0]) --yakus[YAKU_JONCHAN];
	}

	// Hon Ii Sou
	if (IsHoniisou()) {
		yakus[YAKU_HONIISOU]=yakufan[YAKU_HONIISOU];
		if (bak->n_naki[0]) --yakus[YAKU_HONIISOU];
	}

	// Ryanpei
	if (IsRyanpei()) yakus[YAKU_RYANPEI]=yakufan[YAKU_RYANPEI];

	// Chin ii sou
	if (IsChiniisou()) {
		yakus[YAKU_CHINIISOU]=yakufan[YAKU_CHINIISOU];
		if (bak->n_naki[0]) --yakus[YAKU_CHINIISOU];
	}

	// Dora
	yakus[YAKU_DORA] = IsDora() * yakufan[YAKU_DORA];

	// Ura Dora
	yakus[YAKU_URA] = IsUra() * yakufan[YAKU_URA];

	// Aka Dora
	yakus[YAKU_AKA] = IsAka() * yakufan[YAKU_AKA];

	// YAKUMAN

	// Tenhou
	if (IsTenhou()) yakus[YAKU_TENHOU]=yakufan[YAKU_TENHOU];

	// Chihou
	if (IsChihou()) yakus[YAKU_CHIHOU]=yakufan[YAKU_CHIHOU];

	// Da Sangen
	if (IsDasangen()) yakus[YAKU_DASANGEN]=yakufan[YAKU_DASANGEN];

	// Su Anko
	if (IsSuanko()) yakus[YAKU_SUANKO]=yakufan[YAKU_SUANKO];

	// Su Anko Tanki
	if (IsSuanko_tanki()) yakus[YAKU_SUANKO_TANKI]=yakufan[YAKU_SUANKO_TANKI];

	// Tsu ii sou
	if (IsTsuiisou()) {
		yakus[YAKU_TSUIISOU]=yakufan[YAKU_TSUIISOU];
		yakus[YAKU_HONROU]=0;
	}

	// Ryu ii sou
	if (IsRyuiisou()) yakus[YAKU_RYUIISOU]=yakufan[YAKU_RYUIISOU];
	
	// Kokushi musou
	if (IsKokushi()) yakus[YAKU_KOKUSHI]=yakufan[YAKU_KOKUSHI];

	// Kokushi musou 13 machi
	if (IsKokushi_13()) yakus[YAKU_KOKUSHI_13]=yakufan[YAKU_KOKUSHI_13];


	// Da Su Shi
	if (IsDasushi()) yakus[YAKU_DASUSHI]=yakufan[YAKU_DASUSHI];

	// Syao su shi
	if (IsSyaosushi()) yakus[YAKU_SYAOSUSHI]=yakufan[YAKU_SYAOSUSHI];

	// Su kantsu
	if (IsSukantsu()) yakus[YAKU_SUKANTSU]=yakufan[YAKU_SUKANTSU];

	// Chuuren
	int jon;
	jon=IsChuuren();
	if (jon==1) yakus[YAKU_CHUUREN]=yakufan[YAKU_CHUUREN];
	else if(jon==2) yakus[YAKU_JONCHUUREN]=yakufan[YAKU_JONCHUUREN];


	pattern->fan=0;
	for(i=0;i<46;i++)
		pattern->fan+=yakus[i];

	int yakuman_baisu=0;
	for(i=31;i<46;i++) 
		if (yakus[i]) {pattern->isYakuman=true; ++yakuman_baisu;}
	
	// yakuman sub
	if (pattern->isYakuman) {
		for(i=0;i<31;i++)
			yakus[i]=0;
		pattern->fan=13*yakuman_baisu;
		pattern->yakuman_baisu=yakuman_baisu;
	}

	fan_nodora=pattern->fan-yakus[YAKU_DORA] - yakus[YAKU_URA] - yakus[YAKU_AKA];

	pattern->fu_tmp=countFu();
	pattern->fu=pattern->fu_tmp==25?25:(int)ceil((float)pattern->fu_tmp/10)*10;
	countScore();
	for(i=0;i<46;++i)
		pattern->yaku[i]=yakus[i];
}

int Yaku::countFu()
{
	int fu=20;
	if (pattern->isChiitoi) return 25;
	if (yakus[YAKU_PINFU]) return 20;
	if (bak->dacya!=0 && !bak->n_naki[0]) fu+=10;
	if (bak->act==ACT_AGARI_TSUMO) fu += 2;
	if (pattern->atama==bak->bafuu) fu+=2;
	if (pattern->atama==bak->jifuu) fu+=2;
	if (pattern->atama==31 || pattern->atama==32 || pattern->atama==33) fu+=2;
	int i;
	for(i=0;i<34;i++) {
		if(pattern->kotsu[i]) fu += 4*(1+Is19z(i))*(1+3*bak->naki_ankan[0][i]);
		else if (pattern->naki_kotsu[i]) fu += 2*(1+Is19z(i))*(1+3*bak->naki_kan[0][i]);
	}
	if (bak->syanpai==pattern->atama) fu+=2;
	else if (pattern->syuntsu[ 0] && bak->syanpai== 2) fu+=2;
	else if (pattern->syuntsu[ 6] && bak->syanpai== 6) fu+=2;
	else if (pattern->syuntsu[ 9] && bak->syanpai==11) fu+=2;
	else if (pattern->syuntsu[15] && bak->syanpai==15) fu+=2;
	else if (pattern->syuntsu[18] && bak->syanpai==20) fu+=2;
	else if (pattern->syuntsu[24] && bak->syanpai==24) fu+=2;
	else
		for(i=0;i<34;i++)
			if (pattern->syuntsu[i] && bak->syanpai==i+1) {fu+=2;break;}
	if (fu==20 && fan_nodora==1) fu=30;
	return fu;
}

int Yaku::countScore()
{
	int score_tmp;

	if (pattern->isYakuman) score_tmp=8000*pattern->yakuman_baisu;
	else if (pattern->fan>=13) score_tmp=8000;
	else if (pattern->fan>=11) score_tmp=6000;
	else if (pattern->fan>=8) score_tmp=4000;
	else if (pattern->fan>=6) score_tmp=3000;
	else if (pattern->fan==5) score_tmp=2000;
	else if (pattern->fan==4 && pattern->fu>=40) score_tmp=2000;
	else if (pattern->fan==3 && pattern->fu>=70) score_tmp=2000;
	else if (pattern->fan==2 && pattern->fu>=120) score_tmp=2000;
	else score_tmp=pattern->fu*(int)pow(2,(2+pattern->fan));
	
	if (bak->jifuu==27) {
		pattern->score=((int)ceil(((float)score_tmp*6/100)))*100;
		pattern->score_ko=((int)ceil(((float)score_tmp*2/100)))*100;
		pattern->score_oya=0;	
	} else {
		pattern->score=((int)ceil(((float)score_tmp*4/100)))*100;
		pattern->score_oya=((int)ceil(((float)score_tmp*2/100)))*100;
		pattern->score_ko=((int)ceil(((float)score_tmp*1/100)))*100;
	}
	return (int)(pattern->fu * pow(2,2+pattern->fan));
	
}
