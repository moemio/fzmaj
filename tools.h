#ifndef MAJ_TOOLS_H
#define MAJ_TOOLS_H

#include "pointers.h"

namespace FZMAJ_NS {

class Tools : protected Pointers {
public:
	int  Str2pai(char*, int[] );
	int CountPai(int []);
	void randPerm(int, int []);
	int check_aka(int);

	Tools(FZMAJ *maj);
	~Tools();
};

}

#endif
