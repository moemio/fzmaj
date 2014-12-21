#include "stdio.h"
#include "ai.h"
#include "ai_tsumogiri.h"
#include "tools.h"

using namespace FZMAJ_NS;

AI_Tsumogiri::AI_Tsumogiri(FZMAJ *maj) : AI(maj)

{

}

AI_Tsumogiri::~AI_Tsumogiri() {}

void AI_Tsumogiri::compute()
{
//	printf("ai %d computing, dacya %d\n",pos_me,bak->dacya);
	if (bak->dacya == 0)	request(ACT_TSUMOGIRI);
	else request(ACT_CANCEL);
}

void AI_Tsumogiri::settings(int narg, char **arg)
{

}
