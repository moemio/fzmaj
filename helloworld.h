#ifdef COMMAND_CLASS

CommandStyle(helloworld,Helloworld)

#else

#ifndef MAJ_HELLOWORLD_H
#define MAJ_HELLOWORLD_H


#include "pointers.h"

namespace FZMAJ_NS {

class Helloworld : protected Pointers {
public:
	Helloworld(class FZMAJ *);
	void command(int, char **);
};

}

#endif
#endif
