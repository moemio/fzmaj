#include "stdio.h"
#include "ai_general.h"


using namespace FZMAJ_NS;

AI_General::AI_General(FZMAJ *maj) : AI(maj)

{

}

AI_General::~AI_General() {}

void AI_General::compute()
{
	printf ("I'm ai_general, I'm computing.\n");
}

void AI_General::settings(int narg, char **arg)
{

}
