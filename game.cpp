#include "stdio.h"
#include "stdlib.h"
#include "game.h"
#include "tools.h"

using namespace FZMAJ_NS;

Game::Game(FZMAJ *maj) : Pointers(maj) {}

Game::~Game(){}

void Game::init(long s)
{
	int i;
	seed = s;
	srand(seed);
	bafuu = 27;
	kyoku = 0;
	hajioya = rand()%4;
	honba = 0;
	residue = 0;
	pos_ptr = hajioya;
	river.resize(4);
	river_stat.resize(4);
	for(i=0;i<4;++i) {
		score[i]=25000;
	}
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

