#include "stdlib.h"
#include "error.h"

using namespace FZMAJ_NS;

/* ---------------------------------------------------------------------- */

Error::Error(FZMAJ *maj) : Pointers(maj) {}

/* ---------------------------------------------------------------------- */

void Error::all(const char *file, int line, const char *str)
{
    if (screen) fprintf(screen,"ERROR: %s (%s:%d)\n",str,file,line);
    if (logfile) fprintf(logfile,"ERROR: %s (%s:%d)\n",str,file,line);
  	if (screen && screen != stdout) fclose(screen);
  	if (logfile) fclose(logfile);
  	exit(1);
}


/* ---------------------------------------------------------------------- */

void Error::warning(const char *file, int line, const char *str, int logflag)
{
  	if (screen) fprintf(screen,"WARNING: %s (%s:%d)\n",str,file,line);
  	if (logflag && logfile) fprintf(logfile,"WARNING: %s (%s:%d)\n",
                                  str,file,line);
}

/* ---------------------------------------------------------------------- */

void Error::message(const char *file, int line, const char *str, int logflag)
{
  	if (screen) fprintf(screen,"%s (%s:%d)\n",str,file,line);
  	if (logflag && logfile) fprintf(logfile,"%s (%s:%d)\n",str,file,line);
}


/* ---------------------------------------------------------------------- */

void Error::done()
{
  	if (screen && screen != stdout) fclose(screen);
  	if (logfile) fclose(logfile);

  exit(1);
}

void Error::debug(const char *file, int line, const char *str)
{
    if (screen) fprintf(screen,"DEBUG: %s (%s:%d)\n",str,file,line);
  	if (screen && screen != stdout) fclose(screen);
}
