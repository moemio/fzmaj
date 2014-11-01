#ifndef MAJ_ERROR_H
#define MAJ_ERROR_H

#include "pointers.h"

namespace FZMAJ_NS {

class Error : protected Pointers {
 public:
  Error(class FZMAJ *);

  void all(const char *, int, const char *);
  void debug(const char *, int, const char *);
  void warning(const char *, int, const char *, int = 1);
  void message(const char *, int, const char *, int = 1);
  void done();
};

}

#endif
