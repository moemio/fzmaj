#ifndef MAJ_AI_H
#define MAJ_AI_H

#include "pointers.h"
#include "defs.h"

namespace FZMAJ_NS {

class AI : protected Pointers {

public:

	virtual void compute() = 0;
	
	virtual void settings(int, char**) = 0;

	AI(class FZMAJ *);
	virtual ~AI();
	
	class Bakyou *bak;

	int sutehai;
	int aka;      // sutehai is aka
	int act;
	int pos_me;

	// naki_chii
	int cpai1,cpai2,caka;

	void request(int act);

	// 

	void print_tehai();

};

}
#endif
