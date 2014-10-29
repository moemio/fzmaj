#ifndef MAJ_GAME_H
#define MAJ_GAME_H

#include "pointers.h"
#include <vector>
using namespace std;

namespace FZMAJ_NS {

class Game : protected Pointers {
public:


	Game(class FZMAJ *);
	~Game();
//	int getBakyou(int pos, class Bakyou *);
	void start(long);
	void createEmptyBakyou(struct Bakyou *, int);
	int started;

protected:
	
	int pai[136];

	int bafuu;
	int jifuu[4];
	int kyoku;
	int hajioya;
	int oya;
	int honba;
	int residue;
	int n_dora;
	int pai_ptr;
	int jun[4];
	int dead_ptr;
	int pos_ptr;
	int isRiichi[4];
	int n_naki[4];
	int naki_kotsu[4][34];
	int naki_syuntsu[4][34];
	int naki_kan[4][34];
	int naki_ankan[4][34];

	int aka_river[4][3];
	int aka_tehai[4][3];
	int aka_dorahyouji[3];
	int aka_naki[4][3];

	vector< vector<int> > river;
	vector< vector<int> > river_stat;
	vector<int> dora;
	vector<int> ura;
	
	int ura_check;

	int score[4];
	int Ryukyoku;
	int tehai[4][34];

	long seed;

	void clearGame();
	void initGame();
	void initPai(int []);

	int gameLoop();

};

}

#endif
