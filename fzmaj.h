#ifndef MAJ_FZMAJ_H
#define MAJ_FZMAJ_H

#include "stdio.h"

namespace FZMAJ_NS {

class FZMAJ {
public:

	class Memory *memory;
	class Error *error;
	class Input *input;

	FILE *infile;
	FILE *logfile;
	FILE *screen;

	FZMAJ(int, char **);
	~FZMAJ();

	
private:
	
	void help();
	void print_style();
};

}

#endif
