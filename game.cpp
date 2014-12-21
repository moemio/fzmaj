#include "stdio.h"
#include "stdlib.h"
#include "game.h"
#include "tools.h"
#include "input.h"
#include "ai.h"
#include "style_ai.h"
#include "error.h"
#include "agari.h"
#include "syanten.h"
#include "memory.h"

using namespace std;
using namespace FZMAJ_NS;

Game::Game(FZMAJ *maj) : Pointers(maj) {
	
	int i;
	for(i=0;i<4;++i)
		ai[i] = NULL;

	ai_map = new std::map<std::string,AICreator>();
	memory->create(actlist,4,N_ACT,"actlist");

#define AI_CLASS
#define AIStyle(key,Class) \
  (*ai_map)[#key] = &ai_creator<Class>;
#include "style_ai.h"
#undef AIStyle
#undef AI_CLASS
}

Game::~Game(){
	delete ai_map; 
	delete [] pai;
	delete [] ai;
	memory->destroy(actlist);
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
	pai_ptr = 0;
	pos_ptr = hajioya;
	endgame = 0;
	river.resize(4);
	river_stat.resize(4);
	for(i=0;i<4;++i) {
		score[i]=25000;
	}
	
	// seed = 1 for test.

	if (seed==1) {
		initGame();
		is_test=1;
	} else {
		is_test=0;
		printf("Game started. Seed = %d. Hajioya = %d\n",seed, hajioya);
		printf("ptr = %d\n",pai_ptr);
		update_juni();
		printf("juni: %d,%d \n%d,%d \n%d,%d \n%d,%d\n",juni[0],score[juni[0]],
	juni[1],score[juni[1]],juni[2],score[juni[2]],juni[3],score[juni[3]]);
		gameLoop();
			
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
		furiten[i]=0;
	}


	for(i=0;i<3;i++)
		aka_dorahyouji[i]=0;
	dora.clear();
	ura.clear();
	pai_ptr = 0;

	ren_zoku = 0;
	agari_flag = 0;
	n_kan_tot =0;
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
	dora.push_back(c_dora(pai[130]>>2));
	ura.push_back(c_dora(pai[131]>>2));

	aka = tools->check_aka(pai[130]);
	if (aka) aka_dorahyouji[aka-1] = 1;

	for(i=0;i<4;++i) {
		if(ai[i]) {
			createEmptyBakyou(ai[i]->bak,i);
			updateBakyou(ai[i]->bak,i);
		}
	}

	if(seed==1) pai_ptr = 80;
}

int Game::c_dora(int cpai)
{
	if (cpai==8)return 0;
	else if (cpai==17)return 9;
	else if (cpai==26)return 18;
	else if (cpai==30)return 27;
	else if (cpai==33)return 31;
	else return cpai+1;
}
void Game::clear_actlist(int pos)
{
	int i;
	for(i=0;i<N_ACT;++i)
		actlist[pos][i]=0;
}

void Game::make_actlist(int pos)
{
	int i;
	clear_actlist(pos);
	if (cur_pos==pos){
		actlist[pos][ACT_TSUMOGIRI]=1;
		if(!riichi[pos]){
			actlist[pos][ACT_TEKIRI]=1;
			for(i=0;i<34;++i)
				if(tehai[pos][i]==4)actlist[pos][ACT_KAN_SELF]=1;
			if(riichiable(pos))actlist[pos][ACT_RIICHI]=1;
		}
		if(agari->checkAgari(ai[pos]->bak))actlist[pos][ACT_AGARI_TSUMO]=1;
	}
}

void Game::tsumoru(int pos)
{
	int aka,cpai;
	cpai = pai[pai_ptr];
	++pai_ptr;
	aka = tools->check_aka(cpai);
	cur_aka = aka;
	tsumo_hai = cpai>>2;

	++tehai[pos][tsumo_hai];
	if (aka) aka_tehai[pos][aka-1] = 1;
#ifdef GAME_DEBUG
	printf ("pos %d tsumo %d, %s\n", pos, tsumo_hai,tools->Pai2str(tsumo_hai,aka).c_str());
#endif
	make_actlist(pos);
}

void Game::kan_tsumoru(int pos)
{
	int aka, cpai;
	cpai = pai[136-n_dora];
	aka = tools->check_aka(cpai);
	cur_aka = aka;
	tsumo_hai = cpai>>2;

	++tehai[pos][tsumo_hai];
	if (aka) aka_tehai[pos][aka-1] = 1;
#ifdef GAME_DEBUG
	printf ("pos %d rinsyan tsumo %d, %s\n", pos, tsumo_hai, tools->Pai2str(tsumo_hai,aka).c_str());
#endif
	make_actlist(pos);
}

int Game::gameLoop()
{
	int i,j;
	// check ai

	for(i=0;i<4;++i)
		if (!ai[i]) { // error->all(FLERR, "Insufficient AI number");
			if(i==0)create_ai("player",i);
			else create_ai("tsumogiri",i);	
		}
	while(!endgame) {
		initGame();
		printf("start. oya = %d\n",oya);
		if (bafuu==27) printf("Ton %d kyoku.",kyoku+1);
		else if (bafuu==28) printf("Nan %d kyoku.", kyoku+1);
		else if (bafuu==29) printf("Sya %d kyoku.", kyoku+1);
		
		printf ("%d pon ba. residue = %d\n", honba, residue);
	
		cur_pos=oya;
		
	// Main Loop

		while(!Ryukyoku) {
			printf ("********************************************\n");
			printf ("ptr: %d, cur_pos: %d\n",pai_ptr,cur_pos);

#ifdef GAME_DEBUG
			for(j=0;j<4;++j)
				ai[j]->print_tehai();
/*			for(j=0;j<4;++j){
				printf("ai %d's tehai aka 0 is %d\n",j,aka_tehai[j][0]);
				printf("ai %d's tehai aka 1 is %d\n",j,aka_tehai[j][1]);
				printf("ai %d's tehai aka 2 is %d\n",j,aka_tehai[j][2]);
				printf("ai %d's tehai aka 3 is %d\n",j,aka_tehai[j][3]);
			}
*/
#endif
			tsumoru(cur_pos);
			if (Ryukyoku==1)break;
			request_ai(cur_pos);

			if (pai_ptr==dead_ptr) ryukyoku(RYU_NORMAL);
			++cur_pos;
			cur_pos=cur_pos%4;
		}
		update_juni();
	printf("juni: %d,%d \n%d,%d \n%d,%d \n%d,%d\n",juni[0],score[juni[0]],
	juni[1],score[juni[1]],juni[2],score[juni[2]],juni[3],score[juni[3]]);
		for (i=0;i<4;++i)
			if (score[i]<0) endgame = 1;
		if (ren_zoku) {
			if (bafuu==28 && oya == (hajioya+3)%4 && score[oya]>30000 && juni[oya]==1)
				endgame = 1;
			else
				++honba;
			}
		else {
			++kyoku;
			if(kyoku>3){
				kyoku=0;
				if(bafuu==29) endgame = 1;
				else if (bafuu==28) {
					if (score[juni[0]]>30000) endgame = 1;
					else ++bafuu;
				} else ++bafuu;
			++oya;
			oya=oya%4;
			}
		}
	}
	if(residue>0)score[juni[0]] += residue;
	clearGame();
	printf("da wanle.\n"); 
	printf("juni: %d,%d \n%d,%d \n%d,%d \n%d,%d\n",juni[0],score[juni[0]],
	juni[1],score[juni[1]],juni[2],score[juni[2]],juni[3],score[juni[3]]);
	return 0;
}

void Game::update_juni()
{
	int top=0,sec=1,tir=2,las=3,swp;
	if(score[sec]>score[top]){swp=sec;sec=top;top=swp;}
	if(score[tir]>score[top]){swp=tir;tir=top;top=swp;}
	if(score[las]>score[top]){swp=las;las=top;top=swp;}
	if(score[tir]>score[sec]){swp=sec;sec=tir;tir=swp;}
	if(score[las]>score[sec]){swp=las;las=sec;sec=swp;}
	if(score[las]>score[tir]){swp=tir;tir=las;las=swp;}
	juni[0]=top;
	juni[1]=sec;
	juni[2]=tir;
	juni[3]=las;
}

void Game::request(int pos, int ai_act)
{
	switch (ai_act) {
		case ACT_PON:
			if (ponable(pos,sutehai)) add_queue(pos,ACT_PON);
			break;
		case ACT_CHII:
			if (chiiable(pos,sutehai)) add_queue(pos,ACT_CHII);
			break;
		case ACT_KAN:
			if (kanable(pos,sutehai)) add_queue(pos,ACT_KAN);
			break;
		case ACT_KAN_SELF:
			if (kanable(pos,ai[pos]->cpai)) kan(pos);
			else tsumogiri(pos);
			break;
		case ACT_AGARI_RON:
			if (agari->checkAgari(ai[pos]->bak)) add_queue(pos,ACT_AGARI_RON);
			break;
		case ACT_AGARI_TSUMO:
			if (agari->checkAgari(ai[pos]->bak)) agari_tsumo(pos);
			else tsumogiri(pos);
			break;
		case ACT_TSUMOGIRI:
			if (pos == cur_pos) tsumogiri(pos);
			break;
		case ACT_TEKIRI:
			if (pos == cur_pos) tekiri(pos);
			break;
		case ACT_RIICHI:
			if (riichiable(pos)) riichi_sengen(pos);
			else tsumogiri(pos);
			break;
		case ACT_CANCEL:
			break;
	}
	cur_act = ai[pos]->act;
}

int Game::checkRequest(int pos)
{

	int i, p=(pos+1)%4;
	int computeflag[4];
	char* str;
	int len;

	for(i=0;i<4;++i) {
		computeflag[i]=0;
		clear_actlist(i);
	}

	// check tenpai and furiten
		
	if (syanten->is_tenpai(tehai[pos])) {
		agarilist[pos]=syanten->agarilist;
		len=agarilist[pos].length();
		str = (char *)malloc((len)*sizeof(char));
		agarilist[pos].copy(str,len,0);

		check_furiten(pos);
#ifdef GAME_DEBUG
		printf ("pos %d is tenpai.\n",pos);
		if (furiten[pos]) printf( "furiten.\n");
		printf ("machi %s\n",agarilist[pos].c_str());
#endif
	}


	//check ron

	while(p!=pos){
	updateBakyou(ai[p]->bak,p);
	++ai[p]->bak->tehai[sutehai];
	if(agari->checkAgari(ai[p]->bak)){
		computeflag[p]=1;
		actlist[p][ACT_AGARI_RON]=1;
		actlist[p][ACT_CANCEL]=1;
		}
	--ai[p]->bak->tehai[sutehai];
	++p;
	p=p%4;
	}

	//check pon kan chii

	p=(pos+1)%4;
	while(p!=pos){
		if(ponable(p,sutehai) || kanable(p,sutehai)) {
			computeflag[p]=1;
			actlist[p][ACT_PON]=1;
			actlist[p][ACT_CANCEL]=1;
			if(kanable(p,sutehai))actlist[p][ACT_KAN]=1;
#ifdef GAME_DEBUG
			printf("pos %d can request pon/kan.\n",p);
			ai[p]->print_tehai();
#endif
		}
		++p;
		p=p%4;
	}
	p=(pos+1)%4;
	if(chiiable(p,sutehai)) {
		computeflag[p]=1;
		actlist[p][ACT_CHII]=1;
		actlist[p][ACT_CANCEL]=1;
#ifdef GAME_DEBUG
		printf("pos %d can request chii.\n",p);
		ai[p]->print_tehai();
#endif
	}
	
	// compute

	for(i=0;i<3;++i){
		p=(pos+1+i)%4;
		if(computeflag[p]) {
			request_ai(p);
		}
	}
	deal_queue();
}

void Game::ryukyoku(int ryu)
{
	int i,t[4]={0,0,0,0},nt=0;
	switch (ryu) {
		case RYU_NORMAL:
			for(i=0;i<4;++i)
				if (syanten->is_tenpai(tehai[i])) {
					++nt;
					printf("%d tenpai\n",i);
					t[i]=1;
				}
				printf("nt=%d\n",nt);
			if (nt==0) break;
			else if (nt==4) {ren_zoku = 1;break;}
			else {
				for(i=0;i<4;++i)
					if(t[i]) score[i]+=(3000/nt);
					else score[i]-=(3000/(4-nt));
			}
			break;
		case RYU_KYUKYU:
			printf("%d kyukyu ryukyoku\n",cur_pos);
			break;
		case RYU_SUKAN:
			printf("su kan ryukyoku\n");
			break;
			
	}
	Ryukyoku=1;
}

void Game::add_queue(int pos,int ai_act)
{
	printf("in add-queue\n");
	int p = (pos-cur_pos+3)%4;
	printf("p = %d, ai_act = %d\n",p,ai_act);
	if(ai_act==ACT_AGARI_RON) queue[p]=1;
	else if (ai_act==ACT_KAN) queue[3+p]=1;
	else if (ai_act==ACT_PON) queue[6+p]=1;
	else if (ai_act==ACT_CHII) queue[9]=1;
	else error->all(FLERR,"Illegal add_queue command");
	printf("quit queue\n");
}

void Game::deal_queue()
{
	int i,deal_idx = -1;
	int p;
	for(i=0;i<10;++i) 
		if (queue[i]) {
			deal_idx = i;
			break;
		}
	if(deal_idx==-1)return;
	printf("deal queue %d\n",deal_idx);
	for(i=0;i<10;++i)
		queue[i]=0;
	p = ((deal_idx%3)+1+cur_pos)%4;
	printf("cur_pos :%d, pos %d deal index %d\n",cur_pos,p,deal_idx);
	if (deal_idx==9) chii(p,ai[p]->cpai,ai[p]->caka);
	else if (deal_idx>=6) pon(p);
	else if (deal_idx>=3) kan(p);
	else agari_ron(p);
}

void Game::tsumogiri(int pos)
{
	int i;
	if (!tehai[pos][tsumo_hai]){
		// tekiri
		for(i=33;i>=0;--i)
			if(tehai[pos][i]){
				printf("tsumogiri->tekiri %d\n",i);
				ai[pos]->sutehai=i;
				break;
			}
		tekiri(pos);
		return;
	}
	sutehai = tsumo_hai;
	--tehai[pos][sutehai];
	river[pos].push_back(sutehai);
	river_stat[pos].push_back(0);
	if (cur_aka) {
		aka_river[pos][cur_aka-1]=river[pos].size();
		aka_tehai[pos][cur_aka-1]=0;
	}
	++jun[pos];
	dacya = pos;
	printf ("jun %d, ptr = %d, pos %d da %s\n",jun[pos],pai_ptr,pos,tools->Pai2str(sutehai,cur_aka).c_str());
	updateBakyou(ai[pos]->bak, pos);
	checkRequest(pos);
}

void Game::tekiri(int pos)
{
	printf("in here tekiri \n ");
	sutehai = ai[pos]->sutehai;
	--tehai[pos][sutehai];
	river[pos].push_back(sutehai);
	river_stat[pos].push_back(1);
	if (sutehai%9==4 && aka_tehai[pos][(sutehai-4)/9]==1) {
		if (tehai[sutehai]==0) cur_aka = (sutehai-4)/9;
		else cur_aka = ai[pos]->aka;
	} else cur_aka = 0;

	if (ai[pos]->aka) {
		aka_river[pos][cur_aka-1]=river[pos].size();
		aka_river[pos][cur_aka-1]=0;
	}
	++jun[pos];
	furiten[pos]=0;
	dacya = pos;
	printf("jun %d, ptr = %d, pos %d da %s\n",jun[pos],pai_ptr,pos,tools->Pai2str(sutehai,cur_aka).c_str());
	updateBakyou(ai[pos]->bak, pos);
	checkRequest(pos);
}

void Game::check_furiten(int pos)
{
	int i=pos,j;
	if (!furiten[i]) {
		for (j=0;j<river[i].size();++j)
			if (in_agari_list(i,river[i][j]))
				furiten[i]=1;
	}
}

int Game::in_agari_list(int pos,int cpai)
{
	return tenpai[pos][cpai];
}

void Game::add_dora()
{
	dora.push_back(c_dora(pai[130-2*n_dora]>>2));
	ura.push_back(c_dora(pai[131-2*n_dora]>>2));
	++n_dora;
	--dead_ptr;
}

void Game::kan(int pos)
{
	int i;
	int kanpai;
	// minkan
	++n_kan_tot;
	if (cur_pos!=pos) {
		++n_naki[pos];
		++n_naki_kan[pos];
		++n_naki_kotsu[pos];
		naki_kotsu[pos][sutehai]=river[cur_pos].size();
		naki_kan[pos][sutehai]=river[cur_pos].size();
		if (sutehai<27 && sutehai%9==4) aka_naki[pos][(sutehai-4)/9]=1;
		if (cur_aka) aka_river[dacya][cur_aka-1]=0;
		else aka_tehai[pos][(sutehai-4)/9]=0;
		tehai[pos][sutehai]-=3;
		cur_pos = pos;
		for(i=0;i<4;++i)
			++jun[i];
		tehai[pos][sutehai]-=3;
		kan_tsumoru(pos);
		request_ai(pos);
		checkRequest(pos);
		add_dora();
	} else if (cur_pos==pos) {
	// kakan
		if(naki_kotsu[pos][tsumo_hai]) {
			++n_naki_kan[pos];
			naki_kan[pos][tsumo_hai]=river[cur_pos].size();
			if (cur_aka) {
				aka_naki[pos][cur_aka-1]=1;
				aka_tehai[pos][cur_aka-1]=0;
			}
			cur_pos = pos;
			for(i=0;i<4;++i)
				++jun[i];
			kan_tsumoru(pos);
			request_ai(pos);
			checkRequest(pos);
			add_dora();
		} else {
	// ankan
			++n_naki_ankan[pos];
			++n_naki_ankan[pos];
			kanpai = ai[pos]->cpai;
			naki_ankan[pos][kanpai]=river[cur_pos].size();
			if (cur_aka) {
				aka_naki[pos][cur_aka-1]=1;
				aka_tehai[pos][cur_aka-1]=0;
			}
			cur_pos=pos;
			add_dora();
			for(i=0;i<4;++i)
				++jun[i];
			tehai[pos][kanpai]-=4;
			kan_tsumoru(pos);
			request_ai(pos);
			checkRequest(pos);
		}
	}
	int rkflag = 1;
	if (n_dora==5) {
		for(i=0;i<4;++i)
			if(n_naki_kan[i]==4) {
				rkflag = 0;
				break;
			}
		if (rkflag==1) ryukyoku(RYU_SUKAN);
	}
}

void Game::pon(int pos)
{
	int i;
	++n_naki[pos];
	++n_naki_kotsu[pos];
	naki_kotsu[pos][sutehai]=river[cur_pos].size();
	if (cur_aka) {
		aka_naki[pos][cur_aka-1]=1;
		aka_river[dacya][cur_aka-1]=0;
	}
	if (ai[pos]->aka) {
		aka_naki[pos][ai[pos]->aka-1]=1;
		aka_tehai[pos][ai[pos]->aka-1]=0;
	}
	tehai[pos][sutehai] -= 2;
	cur_pos = pos;
	for(i=0;i<4;++i)
		if (i!=pos) ++jun[i];
//	tekiri(pos);
	make_actlist(cur_pos);
	request_ai(cur_pos);
}

void Game::chii(int pos, int cpai, int aka)
{
	int i,p;
	++n_naki[pos];
	p = cpai > sutehai ? sutehai : cpai;
	n_naki_syuntsu[p]=river[cur_pos].size();
	if (cur_aka) {
		aka_naki[pos][cur_aka-1]=1;
		aka_river[dacya][cur_aka-1]=0;
	}
	if (ai[pos]-aka) {
		aka_naki[pos][cur_aka-1]=1;
		aka_tehai[pos][cur_aka-1]=0;
	}
	--tehai[pos][cpai];
	if (sutehai==cpai+1) --tehai[pos][cpai+2];
	else --tehai[pos][cpai+1];
	cur_pos = pos;
	for(i=0;i<4;++i)
		if(i!=pos) ++jun[i];
//	tekiri(pos);
	make_actlist(cur_pos);
	request_ai(cur_pos);
}

void Game::agari_tsumo(int pos)
{
	int i;
	agari->checkAgari(ai[pos]->bak);
	if (pos==oya) 
		for (i=0;i<4;++i)
			if(i!=pos) score[i]-=(agari->score_ko + 100*honba);
			else score[i] += (agari->score_ko*6 + 300*honba);
	else 
		for (i=0;i<4;++i)
			if (i!=pos) 
				if (i==oya) score[i]-=(agari->score_oya +100*honba);
				else score[i]-=(agari->score_ko +100*honba);
			else score[i] += (agari->score_ko*4 + 300*honba);
	score[i] += residue;
	residue = 0;
	// next game
	agari_flag=1;
	if (pos==oya) ren_zoku = 1;
}

void Game::agari_ron(int pos)
{
	agari->checkAgari(ai[pos]->bak);
	score[pos] += (agari->score + 300*honba);
	score[cur_pos] -= (agari->score + 300*honba);
	score[pos] += residue;
	residue = 0;
	agari_flag = 1;
	if (pos==oya) ren_zoku = 1;
}


int Game::kanable(int pos, int pai)
{
	if (n_kan_tot>=4) return 0;
	if (!riichi[pos]) {
		if (pai_ptr == dead_ptr -1) return 0;
		if(cur_pos==pos) return (naki_kotsu[pos][pai] && tehai[pos][pai]) || (tehai[pos][pai]==4);
		else return tehai[pos][pai]==3;
	} else {
		if (cur_pos!=pos)return 0;
		if (pai!=tsumo_hai)return 0;
		if (tehai[pos][pai]!=4)return 0;
		//check tenpai
		string agari_tmp;
		tehai[pos][pai] -= 4;
		syanten->calcSyantenAll(tehai[pos]);
		agari_tmp = syanten->agarilist;
		tehai[pos][pai] += 4;	
		if (agari_tmp==agarilist[pos]) return 1;
		else return 0;
	}
}

int Game::riichiable(int pos)
{
	int rcflag = 0;
	if(score[pos]<1000)return 0;
	if(pai_ptr<dead_ptr-4)return 0;
	--tehai[pos][ai[pos]->sutehai];
	rcflag = syanten->is_tenpai(tehai[pos]);
	++tehai[pos][ai[pos]->sutehai];
	return rcflag;
}

void Game::riichi_sengen(int pos)
{
	int i, tekiri_flag=1;
	if (riichiable(pos)){
		sutehai=ai[pos]->sutehai;
		if (sutehai==-1) {
			sutehai=tsumo_hai;
			tekiri_flag = 0;
		}
		if (tekiri_flag)
			tekiri(pos);
		else tsumogiri(pos);

		riichi[pos]=jun[pos];
		score[pos]-=1000;
		residue += 1000;
	} else
	{
		if(sutehai==-1) tekiri_flag=0;
		if (tekiri_flag) tekiri(pos);
		else tsumogiri(pos);
	}
}

int Game::ponable(int pos, int pai)
{
	return tehai[pos][pai]>=2 && (riichi[pos]==0);
}

int Game::chiiable(int p, int pai)
{
	if (riichi[p]) return 0;
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
	for(j=0;j<4;++j) {
		p = (j-pos+4)%4;
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
		bak->tehai[i]=tehai[pos][i];
	if (cur_pos == pos || cur_act == ACT_KAN) {
		bak->syanpai = tsumo_hai;
		bak->dacya = 0;
		bak->act = 0;
		bak->aka = cur_aka;
	} else {
		bak->syanpai = sutehai;
		bak->dacya = (pos-cur_pos+4)%4;
		bak->act = cur_act;
		bak->aka = cur_aka;
	}
	for (i=0;i<3;++i)
		bak->aka_tehai[i]=aka_tehai[pos][i];
#ifdef GAME_DEBUG
	printf ("bak %d updated. cur_pos = %d, dacya = %d\n",pos,cur_pos,bak->dacya);
#endif
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
		bak->score[i] = score[i];
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
	ai[pos]->pos_me = pos;
	//strcpy(ai[pos]->style,style);
	int n = strlen(style)+1;	
	ai[pos]->style = new char[n];
	strcpy(ai[pos]->style, style);
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

void Game::request_ai(int pos)
{
	updateBakyou(ai[pos]->bak,pos);
	ai[pos]->actlist = actlist[pos];
	ai[pos]->compute();
}

template <typename T>
AI *Game::ai_creator(FZMAJ *maj)
{
	return new T(maj);
}


