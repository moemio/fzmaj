#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "memory.h"
#include "error.h"

using namespace FZMAJ_NS;

Memory::Memory(FZMAJ *maj) : Pointers(maj) {}

void *Memory::smalloc(int nbytes, const char *name)
{
	if (nbytes == 0) return NULL;
 	void *ptr = malloc(nbytes);
	if (ptr == NULL) {
		char str[128];
		sprintf(str,"Failed to allocate %d bytes for array %s",nbytes,name);
		error->all(FLERR,str);
	}
	return ptr;
}

void *Memory::srealloc(void *ptr, int nbytes, const char *name)
{
  if (nbytes == 0) {
    free(ptr);
    return NULL;
  }
  ptr = realloc(ptr,nbytes);
  return ptr;
}

void Memory::sfree(void *ptr)
{
  if (ptr == NULL) return;
  free(ptr);
}

void Memory::free_bakyou(Bakyou *bak)
{
	
}

void Memory::fail(const char *name)
{
	char str[128];
	sprintf(str,"Cannot greate an array of pointers for %s",name);
	error->all(FLERR,str);
}
