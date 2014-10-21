#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "memory.h"

using namespace FZMAJ_NS;

Memory::Memory(FZMAJ *maj) : Pointers(maj) {}

void *Memory::smalloc(int nbytes, const char *name)
{
	if (nbytes == 0) return NULL;
 	void *ptr = malloc(nbytes);
	
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

