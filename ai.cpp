#include "ai.h"
#include "stdio.h"
#include "game.h"
#include "tools.h"
#include "memory.h"

using namespace FZMAJ_NS;

AI::AI(FZMAJ *maj) : Pointers(maj)
{
	bak = new Bakyou;
	memory->create_bakyou(bak);
	memory->create(actlist,N_ACT,"Actlist");
}

AI::~AI() 
{
	memory->destroy(actlist);
	memory->destroy_bakyou(bak);
	delete bak;
}

void AI::request(int ai_act)
{
//#ifdef GAME_DEUBG
	printf("ai %d requesting act %d\n",pos_me,ai_act);
//#endif
	act = ai_act;
	game->request(pos_me, ai_act);
}

void AI::print_tehai()
{
	int i,j,akaflag[3],akap;
	printf("ai %d's tehai = ",pos_me);
	for(i=0;i<3;++i) {
		akaflag[i]=(bak->aka_tehai[i]!=0);
	}
	for(i=0;i<34;++i) {
		if (bak->tehai[i]) {
			for(j=0;j<bak->tehai[i];++j) {
				if (i<27 && i%9==4) {
					akap = (i-4)/9;
					if (akaflag[akap]==1) {
						printf("%s ",tools->Pai2str(i,1).c_str());
						akaflag[akap]=0;
					} else
					printf("%s ",tools->Pai2str(i,0).c_str());  // need check aka
				}
				else
				printf("%s ",tools->Pai2str(i,0).c_str());  // need check aka
			}
		}
	}
	printf("\n");
}

void AI::player_act(int narg, char** arg){}
