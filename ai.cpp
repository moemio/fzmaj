#include "ai.h"
#include "stdio.h"
#include "game.h"
#include "tools.h"

using namespace FZMAJ_NS;

AI::AI(FZMAJ *maj) : Pointers(maj)
{
	bak = new Bakyou;
}

AI::~AI() 
{
	printf("delete bak\n");
	delete bak;
}

void AI::request(int ai_act)
{
	printf("ai %d requesting act %d\n",pos_me,ai_act);
	act = ai_act;
	game->request(pos_me, ai_act);
}

void AI::print_tehai()
{
	int i,j;
	printf("ai %d's tehai = ",pos_me);
	for(i=0;i<34;++i) {
		if (bak->tehai[i]) {
			for(j=0;j<bak->tehai[i];++j) {
				printf("%s ",tools->Pai2str(i,0).c_str());  // need check aka
			}
		}
	}
	printf("\n");
}
