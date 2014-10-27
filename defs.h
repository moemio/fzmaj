#ifndef MAJ_DEFS_H
#define MAJ_DEFS_H

#define PI 3.1415926535897932384626
#define mio "mio hen cong ming de, bu yao qi fu mio."

#include <vector>
using namespace std;


struct Bakyou {
	int bafuu;
	int jifuu;
	int kyoku;
	int oya;
	int honba;
	int residue;
	int n_dora;
	int pai_ptr;
	int dead_ptr;
	int tehai[34];
	// pos: jibun=0; syacha = 1; toicha = 2; syancha = 3
	int jun[4];
	int n_naki[4];
	int naki_kotsu[4][34];
	int naki_syuntsu[4][34];
	int n_naki_syuntsu[4];
	int n_naki_kotsu[4];
	int naki_kan[4][34];
	int naki_ankan[4][34];
	int score[4];
	int riichi[4];
	int aka_river[4][3];
	int aka_naki[4][3];
	int aka_dorahyouji[3];
	int aka_tehai[3];
	vector< vector<int> > river;
	vector< vector<int> > river_stat;
	vector<int> dora;
	int nokori[34];

	int syanpai;
	int dacya;

	int isRed;
};


#endif
