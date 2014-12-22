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
	destroy(bak->tehai);
}

void Memory::create_bakyou(Bakyou* bak)
{
	create(bak->tehai,34,"bak:tehai");
	create(bak->jun,4,"bak:jun");
	create(bak->n_naki,4,"bak:n_naki");
	create(bak->naki_kotsu,4,34,"bak:naki_kotsu");
	create(bak->naki_syuntsu,4,34,"bak:naki_syuntsu");
	create(bak->n_naki_syuntsu,4,"bak:n_naki_syuntsu");
	create(bak->n_naki_kotsu,4,"bak:n_naki_kotsu");
	create(bak->naki_kan,4,34,"bak:naki_kan");
	create(bak->naki_ankan,4,34,"bak:naki_ankan");
	create(bak->n_naki_kan,4,"bak:n_naki_kan");
	create(bak->n_naki_ankan,4,"bak:n_naki_ankan");
	create(bak->score,4,"bak:score");
	create(bak->riichi,4,"bak:riichi");
	create(bak->aka_river,4,3,"bak:aka_river");
	create(bak->aka_naki,4,3,"bak:aka_naki");
	create(bak->aka_dorahyouji,3,"bak:aka_dorahyouji");
	create(bak->aka_tehai,3,"bak:aka_tehai");
	create(bak->nokori,34,"bak:nokori");
}

void Memory::fail(const char *name)
{
	char str[128];
	sprintf(str,"Cannot greate an array of pointers for %s",name);
	error->all(FLERR,str);
}
