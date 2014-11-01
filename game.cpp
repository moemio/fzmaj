#include "stdio.h"
#include "stdlib.h"
#include "game.h"
#include "tools.h"
#include "input.h"

using namespace FZMAJ_NS;

Game::Game(FZMAJ *maj) : Pointers(maj) {}

Game::~Game(){}

void Game::start(long s)
{
	int i;
	seed = s;
	started = 1;
	srand(seed);
	bafuu = 27;
	kyoku = 0;
	hajioya = rand()%4;
	oya = hajioya;
	honba = 0;
	residue = 0;
	pos_ptr = hajioya;
	river.resize(4);
	river_stat.resize(4);
	for(i=0;i<4;++i) {
		score[i]=25000;
	}

	gameLoop();
}

void Game::clearGame()
{
	int i,j;
	
	// clear tehai, river, aka

	for(i=0;i<4;i++) {
		for(j=0;j<34;j++)
			tehai[i][j]=0;
			naki_kotsu[i][j]=0;
			naki_syuntsu[i][j]=0;
			naki_kan[i][j]=0;
			naki_ankan[i][j]=0;
		for(j=0;j<3;j++) {
			aka_tehai[i][j]=0;
			aka_river[i][j]=0;
			aka_naki[i][j]=0;
		}
		river[i].clear();
		river_stat[i].clear();
		jun[i]=0;
		isRiichi[i]=0;
	}


	for(i=0;i<3;i++)
		aka_dorahyouji[i]=0;
	dora.clear();
	ura.clear();

}

void Game::initGame()
{
	int i,j,k,p,c0,c1,aka;

	clearGame();

	// init yama
	
	tools->randPerm(136, pai);

	//4*3

	for(i=0;i<3;i++)
		for(j=0;j<4;j++) {
			p = (j+oya)%4;
			for(k=0;k<4;k++) {
				c0 = pai[pai_ptr+k];
				c1 = c0>>2;
				++tehai[p][c1];
				aka = tools->check_aka(c0);
				if (aka) aka_tehai[p][aka-1]=1;
			}
			pai_ptr += 4;
		}
	
	// 1*1

	for(i=0;i<4;i++) {
		p = (i+oya)%4;
		c0 = pai[pai_ptr+i];
		c1 = c0>>2;
		++tehai[p][c1];
		aka = tools->check_aka(c0);
		if (aka) aka_tehai[p][aka-1]=1;
	}
	pai_ptr += 4;

	pai_ptr = 0;
	dead_ptr = 122;
	ura_check = 0;
	Ryukyoku  = 0;

	n_dora = 1;
	dora.push_back(pai[131]>>2);
	ura.push_back(pai[132]>>2);

	aka = tools->check_aka(pai[131]);
	if (aka) aka_dorahyouji[aka-1]=1;

}

int Game::gameLoop()
{
	int kr;
	
	initGame();
/*	printf("start. oya = %d\n",oya);

	while(!Ryukyoku) {
		
		++pai_ptr;

		if (pai_ptr==dead_ptr) Ryukyoku=1;
	}

	clearGame();
	printf("da wanle.\n"); */
}

void Game::createEmptyBakyou(Bakyou *bak, int pos)
{
	int i,j;
	bak->bafuu = bafuu;
	bak->jifuu = 27+(4+pos-oya)%4;
	bak->kyoku = kyoku;
	bak->oya = oya;
	bak->honba = 0;
	bak->residue = 0;
	bak->n_dora = 0;
	bak->pai_ptr = pai_ptr;
	bak->dead_ptr = dead_ptr;
	bak->river.resize(4);
	bak->river_stat.resize(4);
	for(i=0;i<4;++i) {
		bak->river[i].clear();
		bak->river_stat[i].clear();
		bak->jun[i] = 0;
		bak->n_naki[i] = 0;
		bak->n_naki_syuntsu[i] = 0;
		bak->n_naki_kotsu[i] = 0;
		bak->n_naki_kan[i] = 0;
		bak->riichi[i] = 0;
		bak->score[i] = 25000;
		for(j=0;j<34;++j) {
			bak->naki_kotsu[i][j] = 0;
			bak->naki_syuntsu[i][j] = 0;
			bak->naki_kan[i][j] = 0;
			bak->naki_ankan[i][j] = 0;
		}
		for(j=0;j<3;++j) {
			bak->aka_river[i][j] = 0;
			bak->aka_naki[i][j] = 0;
		}
	}
	for(i=0;i<3;++i) {
		bak->aka_dorahyouji[i] = 0;
		bak->aka_tehai[i] = 0;
	}
	bak->dora.clear();
	bak->ura.clear();
	for(i=0;i<34;++i) {
		bak->nokori[i] = 4;
		bak->tehai[i] = 0;
	}
	bak->syanpai = 0;
	bak->dacya = 0;
	bak->act = 0;
	bak->isRed = 0;
}		
