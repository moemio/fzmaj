#ifndef MAJ_AI_H
#define MAJ_AI_H

#include "pointers.h"
#include "defs.h"
#include <string>

namespace FZMAJ_NS {

class AI : protected Pointers {

public:

	int pos_me;
	char* style;

	virtual void player_act(int,char**);
	virtual void compute() = 0;
	virtual void settings(int, char**) = 0;
	void request(int act);
	void print_tehai();

	AI(class FZMAJ *);
	virtual ~AI();
	
	struct Bakyou *bak;

	int sutehai;
	int aka;      // sutehai is aka
	int act;

	int *actlist;
	
	// naki_chii
	int cpai,caka;

	struct Pai{
		int idx;
		int aka;
		string name;
		int score;
	};

	void tehai_14();
	vector<Pai>th;
	void paili();
	void collect_nokori();


};

}
#endif
