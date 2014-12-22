#ifndef MAJ_MEMORY_H
#define MAJ_MEMORY_H

#include "pointers.h"
#include "defs.h"

namespace FZMAJ_NS {

class Memory : protected Pointers {
public:
	Memory(class FZMAJ *);

	void *smalloc(int n, const char *);
	void *srealloc(void *, int n, const char *);
	void sfree(void *);
	void free_bakyou(class Bakyou *);
	void create_bakyou(class Bakyou*);
	void fail(const char *);


	template <typename TYPE>
	TYPE *create(TYPE *&array, int n, const char *name)
	{
		int nbytes = ((int) sizeof(TYPE))*n;
		array = (TYPE *) smalloc(nbytes,name);
		return array;
	}

  	template <typename TYPE>
  	TYPE **create(TYPE **&array, int n, const char *name) 
  	{fail(name); return NULL;}
	
	template <typename TYPE>
	TYPE **create(TYPE **&array, int n1, int n2, const char *name)
	{
		int nbytes = ((int) sizeof(TYPE)) * n1 * n2;
		TYPE *data = (TYPE *) smalloc(nbytes,name);
		nbytes = ((int) sizeof(TYPE *)) * n1;
		array = (TYPE **) smalloc(nbytes,name);
	
		int n=0;
		for(int i=0;i<n1;i++) {
			array[i] = &data[n];
			n += n2;
		}
		return array;
	}

	template <typename TYPE>
	void destroy(TYPE *array)
	{sfree(array);}

	template <typename TYPE>
	void destroy(TYPE **array)
	{
		if (array==NULL) return;
		sfree(array[0]);
		sfree(array);
	}

};

}

#endif


