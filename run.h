#ifdef COMMAND_CLASS

CommandStyle(run,Run)

#else

#ifndef MAJ_RUN_H
#define MAJ_RUN_H

#include "pointers.h"

namespace FZMAJ_NS {

class Run : protected Pointers {
 public:
  Run(class FZMAJ *);
  void command(int, char **);
};

}

#endif
#endif
