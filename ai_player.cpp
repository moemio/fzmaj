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
	int i,j,p,akap;
	printf("ai %d computing, dacya %d\n",pos_me,bak->dacya);
	printf("my tehai:\n");
	print_tehai();
	for (i=0;i<4;++i){
		printf("river %d is :",i);
		for(j=0;j<bak->river[i].size();++j) {
			p=bak->river[i][j];
			if(p<27 && p%9==4) {
				akap = (p-4)/9;
				if (bak->aka_river[i][akap]==j+1)
					printf("%s ",tools->Pai2str(p,1).c_str());
				else
					printf("%s ",tools->Pai2str(p,0).c_str());
			} else
			printf("%s ",tools->Pai2str(p,0).c_str());
		}
		printf("\n");
	}

	if (bak->dacya == 0)	request(ACT_TSUMOGIRI);
	else request(ACT_CANCEL);
}

void AI_Player::settings(int narg, char **arg)
{

}
