#ifndef MAJ_AGARI_H
#define MAJ_AGARI_H

#include "pointers.h"
#include "defs.h"
using namespace std;

namespace FZMAJ_NS {

class Agari : protected Pointers {

public:

	Bakyou *bak;
	vector<PATTERN> pattern;

	int fan;
	int fu;
	int score;
	int score_ko;
	int score_oya;
	int agari_yaku[46];
	

	int n_syuntsu;
	int n_kotsu;
	int n_mentsu;
	int atama;
	int isatama;
	int kotsu[34];
	int syuntsu[34];
	
	Agari(class FZMAJ * );
	~Agari();
	
	int checkAgari(Bakyou *);
	int agari_test( int []);
	int check_agari_empty(int [],int);

	int maxp;
protected:
	
	int c[34];
	int back_up_c[34];
	void d_kotsu(int k);
	void i_kotsu(int k);
	void d_syuntsu(int k);
	void i_syuntsu(int k);
	void d_atama(int k);
	void i_atama(int k);
	int cc2m(int [],int);
	bool isAtamaMentsu(int,int);
	bool isMentsu(int);

	void init();

	void removeJihai();
	void updateResult();
	int tokuHandan();

	void Run(int depth);
	void printPattern(int);

	string hai2str(int);
};

}

#endif
