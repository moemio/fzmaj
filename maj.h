#ifndef MAJ_MAJ_H
#define MAJ_MAJ_H

#include "pointers.h"

namespace FZMAJ_NS {

class MAJ : procted Pointers {
public:
	
	long seed;

	int bafuu;
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
	vector< vector<int> > river;

	int n_naki[4];
	int naki_kotsu[4];
	int naki_syuntsu[4];
	int naki_kan[4];

	vector<int> dora;

	int score[4];

	int clearGame();
	int Ryukyoku;
	
	int initGame();
	int startGame();


	

protected:

	vector<int> ura;


	void initPai(int []);
	void randPerm(int, int []);

};

}

#endif
