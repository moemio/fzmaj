#ifndef MAJ_TOOLS_H
#define MAJ_TOOLS_H

#include "pointers.h"
#include <string>

namespace FZMAJ_NS {

class Tools : protected Pointers {
public:
	int  Str2pai(char*, int[] );
	std::string Pai2str(int, int);
	int CountPai(int []);
	void randPerm(int, int []);
	int check_aka(int);
	void print_tehai(int []);

	Tools(FZMAJ *maj);
	~Tools();
};

}

#endif
