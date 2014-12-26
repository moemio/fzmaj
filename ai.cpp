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
}

AI::~AI() 
{
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


void AI::tehai_14()
{
	int i,j,p=0,akaflag[3];
	int nc;
	int sute_flag=0;
	for(i=0;i<3;++i)
		if(bak->aka_tehai[i])akaflag[i]=1;
		else akaflag[i]=0;
	nc = tools->CountPai(bak->tehai);
	if (nc%3==2){
	   	sute_flag=1;
		--bak->tehai[bak->syanpai];
	}
	for(i=0;i<34;++i) {
		if(bak->tehai[i]){
			for(j=0;j<bak->tehai[i];++j){
				th[p].idx = i;
				if(i<27 && i%9==4 && akaflag[(i-4)/9]==1) {
					if(bak->aka!=(i-4)/9+1)	th[p].aka = (i-4)/9+1;
					akaflag[(i-4)/9]=0;
				} else th[p].aka = 0;
				th[p].name = tools->Pai2str(th[p].idx,th[p].aka);
			++p;
			}
		}
	}
	if (sute_flag) {
		th[p].idx = bak->syanpai;
		th[p].aka = bak->aka;
		th[p].name = tools->Pai2str(th[p].idx,th[p].aka);
		++bak->tehai[bak->syanpai];
	}
	for(i=0;i<nc;++i)
		printf("%4d",i+1);
	printf("\n");
	for(i=0;i<nc;++i)
		printf("%4s",th[i].name.c_str());
	printf("\n");
}

void AI::paili()
{

}

void AI::collect_nokori()
{
	int i,j,nj;

// clean nokori

	for(i=0;i<34;++i)
		bak->nokori[i]=4;
	
	for(i=0;i<4;++i) {
		nj=bak->river[i].size();
		for(j=0;j<nj;++j)
			--bak->nokori[bak->river[i][j]];
	}

	for(i=0;i<34;++i) {
		bak->nokori[i]-=bak->tehai[i];
		for(j=0;j<4;++j) {
			if(bak->naki_kotsu[j][i])bak->nokori[i]-=3;
			if(bak->naki_kan[j][i])bak->nokori[i]-=1;
			if(bak->naki_ankan[j][i])bak->nokori[i]-=1;
			if(bak->naki_syuntsu[j][i])bak->nokori[i]-=1;
			if(bak->naki_syuntsu[j][i-1] && i<27 && i%9>0)bak->nokori[i]-=1;
			if(bak->naki_syuntsu[j][i-2] && i<27 && i%9>1)bak->nokori[i]-=1;
		}
	}	

}
