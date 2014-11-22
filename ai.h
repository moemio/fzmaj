#ifndef MAJ_AI_H
#define MAJ_AI_H

#include "pointers.h"
#include "defs.h"

namespace FZMAJ_NS {

class AI : protected Pointers {

public:
	double eng_tot[34];

	virtual void compute() = 0;
	
	virtual void settings(int, char**) = 0;

	AI(class FZMAJ *);
	virtual ~AI();
	
	class Bakyou *bak;

	int sutehai;
	int aka;
	int act;
	int pos_me;

	void request(int act);

};

}
#endif
