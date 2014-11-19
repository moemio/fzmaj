#include "stdio.h"
#include "ai_tsumogiri.h"


using namespace FZMAJ_NS;

AI_Tsumogiri::AI_Tsumogiri(FZMAJ *maj) : AI(maj)

{

}

AI_Tsumogiri::~AI_Tsumogiri() {}

void AI_Tsumogiri::compute()
{

	printf ("I'm ai_tsumogiri, I'm computing.\n");
}

void AI_Tsumogiri::settings(int narg, char **arg)
{

}
