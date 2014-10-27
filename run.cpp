#include "run.h"
#include "stdlib.h"
#include "input.h"
#include "game.h"
#include "error.h"

using namespace FZMAJ_NS;

Run::Run(FZMAJ *maj) : Pointers(maj) {}

void Run::command(int narg, char **arg)
{
	printf("narg=%d\n",narg);

	if (narg<1) error->all(FLERR, "Need random seed.");
	long seed = atol(arg[0]);
	game->start(seed);
}
