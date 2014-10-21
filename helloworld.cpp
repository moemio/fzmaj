#include "stdio.h"
#include "helloworld.h"

using namespace FZMAJ_NS;

Helloworld::Helloworld(FZMAJ *maj) : Pointers(maj) {}

void Helloworld::command(int narg, char **arg)
{
	if (screen) fprintf(screen, "Helloworld!\n");
}
