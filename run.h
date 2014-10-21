#ifndef LMP_RUN_H
#define LMP_RUN_H

#include "pointers.h"

namespace FZMAJ_NS {

class Run : protected Pointers {
 public:
  Run(class FZMAJ *);
  void command(int, char **);
};

}

#endif
