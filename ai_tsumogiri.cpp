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
	request(ACT_TSUMOGIRI);
}

void AI_Tsumogiri::settings(int narg, char **arg)
{

}
