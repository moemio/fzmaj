#ifndef MAJ_POINTERS_H
#define MAJ_POINTERS_H

#include "fzmaj.h"
#include "defs.h"

namespace FZMAJ_NS {

#define FLERR __FILE__,__LINE__

#define MIN(A,B) ((A) < (B) ? (A) : (B))
#define MAX(A,B) ((A) > (B) ? (A) : (B))


class Pointers {

public:
	Pointers(FZMAJ *ptr) :
		maj(ptr),
		memory(ptr->memory),
		error(ptr->error),
		input(ptr->input),
		syanten(ptr->syanten),
		infile(ptr->infile),
		screen(ptr->screen),
		logfile(ptr->logfile) {}
	virtual ~Pointers() {}

protected:
	
	FZMAJ *maj;
	Memory *&memory;
	Error *&error;
	Input *&input;

	Syanten *&syanten;

	FILE *&infile;
	FILE *&logfile;
	FILE *&screen;
};

}

#endif
