#ifndef MAJ_MEMORY_H
#define MAJ_MEMORY_H

#include "pointers.h"

namespace FZMAJ_NS {

class Memory : protected Pointers {
public:
	Memory(class FZMAJ *);

	void *smalloc(int n, const char *);
	void *srealloc(void *, int n, const char *);
	void sfree(void *);

};

}

#endif
