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
	int *tehai;
	// pos: jibun=0; syacha = 1; toicha = 2; syancha = 3
	int *jun;
	int *n_naki;
	int **naki_kotsu;
	int **naki_syuntsu;
	int *n_naki_syuntsu;
	int *n_naki_kotsu;
	int **naki_kan;
	int **naki_ankan;
	int *n_naki_kan;
	int *n_naki_ankan;
	int *score;
	int *riichi;
	int **aka_river;
	int **aka_naki;
	int *aka_dorahyouji;
	int *aka_tehai;
	vector< vector<int> > river;
	vector< vector<int> > river_stat;
	vector<int> dora;
	vector<int> ura;
	int *nokori;

	int syanpai;
	int dacya;
	int act;

	int aka;   // syanpai is aka
};

struct PATTERN {
   int c[34];
   int score;
   int score_oya;
   int score_ko;
   int kotsu[34];
   int syuntsu[34];
   int toitsu[34];
   int atama;
   int n_kotsu;
   int n_syuntsu;
   int n_naki;
   int naki_kotsu[34];
   int naki_syuntsu[34];
   int n_naki_syuntsu;
   int n_naki_kotsu;
   int isYakuman;
   int yakuman_baisu;
   int isChiitoi;
   int isKokushi;
   int fan;
   int fu;
   int fu_tmp;
   int yaku[46];
};

enum ENUM_ACT {
	ACT_TSUMORU = 0,
	ACT_TEKIRI,
	ACT_TSUMOGIRI,
	ACT_AGARI_TSUMO,
	ACT_AGARI_RON,
	ACT_CHII,
	ACT_PON,
	ACT_KAN,
	ACT_KAN_SELF,
	ACT_RIICHI,
	ACT_CANCEL,
	ACT_NAGASHI,
	N_ACT
};

enum ENUM_RYUKYOKU {
	RYU_NORMAL = 0,
	RYU_KYUKYU,
	RYU_SUKAN,
	RYU_SANKARI
};

const char ACT_NAME[12][128] = {
	{"ACT_Tsumoru		"},
	{"ACT_Tekiri		"},
	{"ACT_Tsumogiri		"},
	{"ACT_Agari_Tsumo	: tsumo	"},
	{"ACT_Agari_Ron		: ron	"},
	{"ACT_Chii			: chii	"},
	{"ACT_Pon			: pon	"},
	{"ACT_Kan			: kan	"},
	{"ACT_Kan_self		: ankan "},
	{"ACT_Riichi 		: riichi"},
	{"ACT_Cancel		"},
	{"ACT_Nagashi		: nagare"}
};

#endif
