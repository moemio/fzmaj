#include "stdio.h"
#include "stdlib.h"
#include "game.h"
#include "tools.h"
#include "input.h"
#include "ai.h"
#include "style_ai.h"
#include "error.h"
#include "agari.h"
#define GAME_DEBUG

using namespace std;
using namespace FZMAJ_NS;

Game::Game(FZMAJ *maj) : Pointers(maj) {
	
	int i;
	for(i=0;i<4;++i)
		ai[i] = NULL;
	char *str = (char *) "none";
	int n = strlen(str) + 1;
//	ai_style = new char[n];
//	strcpy(ai_style,str);

	ai_map = new std::map<std::string,AICreator>();

#define AI_CLASS
#define AIStyle(key,Class) \
  (*ai_map)[#key] = &ai_creator<Class>;
#include "style_ai.h"
#undef AIStyle
#undef AI_CLASS
}

Game::~Game(){
	int i;
	for(i=0;i<4;++i)
		if(ai[i]) delete ai[i];
//	delete ai_map; // Segmentation fault
}

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
	
	// seed = 1 for test.

	if (seed==1) 
		initGame();
	else {
		printf("Game started. Seed = %d. Hajioya = %d\n",seed, hajioya);
		init_ai();
		gameLoop();
			
	}
	printf ("pass start\n");
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
		n_naki[i] = 0;
		n_naki_syuntsu[i] = 0;
		n_naki_kotsu[i] = 0;
		n_naki_kan[i] = 0;
		n_naki_ankan[i] = 0;
		for(j=0;j<3;j++) {
			aka_tehai[i][j]=0;
			aka_river[i][j]=0;
			aka_naki[i][j]=0;
		}
		river[i].clear();
		river_stat[i].clear();
		jun[i]=0;
		riichi[i]=0;
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

	dead_ptr = 122;
	ura_check = 0;
	Ryukyoku  = 0;

	n_dora = 1;
	dora.push_back(pai[130]>>2);
	ura.push_back(pai[131]>>2);

	aka = tools->check_aka(pai[130]);
	if (aka) aka_dorahyouji[aka] = 1;

}

void Game::tsumoru(int pos)
{
	int aka,cpai;
	cpai = pai[pai_ptr];
	++pai_ptr;
	aka = tools->check_aka(cpai);
	cur_aka = aka;
	tsumo_hai = cpai>>2;

	++tehai[pos][cpai];
	if (aka) aka_tehai[pos][aka-1] = 1;
#ifdef GAME_DEBUG
	printf ("pos %d tsumo %d, %s\n", pos, tsumo_hai,tools->Pai2str(tsumo_hai,aka).c_str());
#endif
}

int Game::gameLoop()
{
	int kr, endgame=0;
	int i;
	int p;
	// check ai

	for(i=0;i<3;++i)
		if (!ai[i]) error->all(FLERR, "Insufficient AI number");
	
	while(!endgame) {
		initGame();
		printf("start. oya = %d\n",oya);
		if (bafuu==27) printf("Ton %d kyoku.",kyoku+1);
		else if (bafuu==28) printf("Nan %d kyoku.", kyoku+1);
		else if (bafuu==29) printf("Sya %d kyoku.", kyoku+1);
		
		printf ("%d pon ba. residue = %d\n", honba, residue);
	
		p=oya;
		while(!Ryukyoku) {
			cur_pos = p;	
			tsumoru(p);
			updateBakyou(ai[p]->bak, p);
			ai[p]->compute();

			checkRequest(p);

			if (pai_ptr==dead_ptr) Ryukyoku=1;
			++p;
			p=p%4;
		}
		endgame = 1;
	}

	clearGame();
	printf("da wanle.\n"); 
	return 0;
}

void Game::dealRequest(int pos, int ai_act)
{
	switch (ai_act) {
		case ACT_TSUMOGIRI:
			sutehai = tsumo_hai;
			--tehai[pos][sutehai];
			river[pos].push_back(sutehai);
			river_stat[pos].push_back(0);
			if (cur_aka) {
				aka_river[pos][ai[pos]->aka-1]=1;
			}
			++jun[pos];
			dacya = pos;
			printf ("jun %d, ptr = %d, pos %d da %s\n",jun[pos],pai_ptr,pos,tools->Pai2str(sutehai,cur_aka).c_str());
			break;

	}
	cur_act = ai[pos]->act;
}

void Game::request(int pos, int ai_act)
{
	switch (ai_act) {
		case ACT_CHII:
			if (chiiable(pos,sutehai)) dealRequest(pos,ai_act);
			else ai_act = ACT_CANCEL;
			break;
		case ACT_PON:
			if (ponable(pos,sutehai)) dealRequest(pos,ai_act);
			else ai_act = ACT_CANCEL;
			break;
		case ACT_TSUMOGIRI:
			dealRequest(pos,ai_act);
	}
}

void Game::checkRequest(int pos)
{
	int p=(pos+1)%4;
// check agari
	while(p!=pos){
	updateBakyou(ai[p]->bak,p);
	++ai[p]->bak->tehai[sutehai];
	if(agari->checkAgari(ai[p]->bak)){
		ai[p]->bak->act = ACT_AGARI_RON;
		ai[p]->compute();
		}
	++p;
	p=p%4;
	}
		
// check pon

// check chii
}

void Game::pon(int pos)
{

}

void Game::chii(int pos)
{

}

void Game::agari_tsumo(int pos)
{

}

void Game::agari_ron(int pos)
{

}

void Game::kan(int pos)
{

}

int Game::kanable(int pos, int pai)
{
	if (pai_ptr == dead_ptr -1) return 0;
	if(cur_pos==pos) return (naki_kotsu[pos][pai] && tehai[pos][pai]) || (tehai[pos][pai]==4);
	else return tehai[pos][pai]==3;
}

int Game::ponable(int pos, int pai)
{
	return tehai[pos][pai]>=2;
}

int Game::chiiable(int p, int pai)
{
	if ((cur_pos+1)%4 != p) return 0;
	if(pai>=27) return 0;
	if(pai%9==0 || pai%9==8)
		return 
	  (pai== 0 && tehai[p][ 1] && tehai[p][ 2]) || (pai== 8 && tehai[p][ 7] && tehai[p][ 6]) ||
	  (pai== 9 && tehai[p][10] && tehai[p][11]) || (pai==17 && tehai[p][16] && tehai[p][15]) ||
	  (pai==18 && tehai[p][19] && tehai[p][20]) || (pai==26 && tehai[p][25] && tehai[p][24]);
	else if (pai%9==1 || pai%9==7) 
		if (tehai[p][pai-1] & tehai[p][pai+1]) return 1;
		else return
	  (pai== 1 && tehai[p][ 2] && tehai[p][ 3]) || (pai== 7 && tehai[p][ 6] && tehai[p][ 5]) ||
	  (pai==10 && tehai[p][11] && tehai[p][12]) || (pai==16 && tehai[p][15] && tehai[p][14]) ||
	  (pai==19 && tehai[p][20] && tehai[p][21]) || (pai==25 && tehai[p][24] && tehai[p][23]);
	else return (tehai[p][pai-1] && tehai[p][pai+1]) ||
		   (tehai[p][pai-1] && tehai[p][pai-2]) ||
		   (tehai[p][pai+1] && tehai[p][pai+2]);
}

void Game::initBakyou(Bakyou *bak, int pos)
{
	int i,k;
	bak->bafuu = bafuu;
	bak->jifuu = 27 + (4+pos-oya)%4;
	bak->oya = oya;
	bak->kyoku = kyoku;
	bak->honba = honba;
	bak->residue = residue;
	bak->pai_ptr = pai_ptr;
	bak->dead_ptr = dead_ptr;
	bak->n_dora = n_dora;
	bak->dora.push_back(dora[0]);
	for(i=0;i<3;++i)
		bak->aka_dorahyouji[i] = aka_dorahyouji[i];
	--bak->nokori[bak->dora[0]];
	for(i=0;i<34;++i)
		bak->nokori[i] -= tehai[pos][i];
	for(i=0;i<4;++i)
		bak->score[i] = score[(i+pos)%4];
}

void Game::updateBakyou(Bakyou *bak, int pos)
{
	int i,j,p;
	for(j=0;j<3;++j) {
		p = (pos+j)%4;
		bak->river[j] = river[p];
		bak->river_stat[j] = river[p];
		bak->jun[j] = jun[p];
		bak->n_naki[j] = n_naki[p];
		bak->n_naki_syuntsu[j] = n_naki_syuntsu[p];
		bak->n_naki_kotsu[j] = n_naki_kotsu[p];
		bak->n_naki_kan[j] = n_naki_kan[p];
		bak->n_naki_ankan[j] = n_naki_ankan[p];
		bak->riichi[j] = riichi[p];
		if(n_naki[p]) {
			if(n_naki_syuntsu[p])
				for(i=0;i<34;++i)
					bak->naki_syuntsu[j][i] = naki_syuntsu[p][i];
			if(n_naki_kotsu[p])
				for(i=0;i<34;++i)
					bak->naki_kotsu[j][i] = naki_kotsu[p][i];
			if(n_naki_kan[p])
				for(i=0;i<34;++i)
					bak->naki_kan[j][i] = naki_kan[p][i];
			if(n_naki_ankan[p])
				for(i=0;i<34;++i)
					bak->naki_ankan[j][i] = naki_kan[p][i];
		}
		for(i=0;i<3;++i) {
			bak->aka_river[j][i] = aka_river[p][i];
			bak->aka_naki[j][i] = aka_naki[p][i];
		}
	}
	for(i=0;i<34;++i)
		bak->tehai[i]=tehai[p][i];
	if (cur_pos == pos || cur_act == ACT_KAN_SELF) {
		bak->syanpai = tsumo_hai;
		bak->dacya = 0;
		bak->act = 0;
		bak->aka = cur_aka;
	} else {
		bak->syanpai = sutehai;
		bak->dacya = cur_pos;
		bak->act = cur_act;
		bak->aka = cur_aka;
	}
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
		bak->n_naki_ankan[i] = 0;
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
	bak->aka = 0;
}		

void Game::create_ai(const char *style, int pos)
{
	if (ai[pos]) delete ai[pos];
	ai[pos] = new_ai(style);
	ai[pos]-> pos_me = pos;
}

void Game::init_ai()
{
	int i;
	for(i=0;i<4;++i) {
		ai[i]->bak = new Bakyou;
		createEmptyBakyou(ai[i]->bak, i);
	}
}

AI *Game::new_ai(const char *style)
{
	if (!strcmp(style, "none")) return NULL;
	if (ai_map->find(style) != ai_map->end()) {
		AICreator ai_creator = (*ai_map)[style];
		return ai_creator(maj);
	}

	error->all(FLERR, "Invalid ai style");
	return NULL;
}

template <typename T>
AI *Game::ai_creator(FZMAJ *maj)
{
	return new T(maj);
}


