#include "stdio.h"
#include "ai.h"
#include "ai_player.h"
#include "tools.h"

using namespace FZMAJ_NS;

AI_Player::AI_Player(FZMAJ *maj) : AI(maj)

{

}

AI_Player::~AI_Player() {}

void AI_Player::compute()
{
	printf("ai %d computing, dacya %d\n",pos_me,bak->dacya);
	if (bak->dacya == 0)	request(ACT_TSUMOGIRI);
	else request(ACT_CANCEL);
}

void AI_Player::settings(int narg, char **arg)
{

}
