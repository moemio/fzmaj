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
	int yaku[46];
	
	int c[34];

	int n_syuntsu;
	int n_kotsu;
	int n_mentsu;
	int atama;
	int isatama;
	int kotsu[34];
	int syuntsu[34];
	
	Agari(class FZMAJ * );
	~Agari();
	
	void checkAgari(Bakyou *);

protected:
	int maxp;
	
	void d_kotsu(int k);
	void i_kotsu(int k);
	void d_syuntsu(int k);
	void i_syuntsu(int k);
	void d_atama(int k);
	void i_atama(int k);
	int cc2m(int);
	bool isAtamaMentsu(int,int);
	bool isMentsu(int);

	void removeJihai();
	void updateResult();
	void tokuHandan();

	void countScore(PATTERN &part);
	void Run(int depth);
};

}

#endif
