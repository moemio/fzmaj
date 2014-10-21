#include "run.h"
#include "input.h"

using namespace FZMAJ_NS;

Run::Run(FZMAJ *maj) : Pointers(maj) {}

void Run::command(int narg, char **arg)
{
	printf("Hello big world!\n");
}
