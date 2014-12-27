#include "stdio.h"
#include "ai.h"
#include "ai_player.h"
#include "tools.h"
#include "input.h"
#include "stdlib.h"
#include "string.h"
#include <iostream>

using namespace FZMAJ_NS;
using namespace std;

AI_Player::AI_Player(FZMAJ *maj) : AI(maj)

{
}

AI_Player::~AI_Player() {
}

void AI_Player::compute()
{
	int i,j,k,p,akap;
	printf("ai %d computing, dacya %d\n",pos_me,bak->dacya);
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
	for(i=0;i<4;++i){
		printf("naki %d is : ",i);
		for(j=0;j<34;++j){
			if(bak->naki_ankan[i][j])printf("##%s%s## ", tools->Pai2str(j,0).c_str(),tools->Pai2str(j,0).c_str());
			else if (bak->naki_kan[i][j])printf("%s%s%s%s ",tools->Pai2str(j,0).c_str(),tools->Pai2str(j,0).c_str(),
				tools->Pai2str(j,0).c_str() ,tools->Pai2str(j,0).c_str());
			else if(bak->naki_kotsu[i][j])printf("%s%s%s ",tools->Pai2str(j,0).c_str(),tools->Pai2str(j,0).c_str(),tools->Pai2str(j,0).c_str());
			if(bak->naki_syuntsu[i][j])
				for(k=0;k<bak->naki_syuntsu[i][j];++k)
				printf("%s%s%s ",tools->Pai2str(j,0).c_str(),tools->Pai2str(j+1,0).c_str(),tools->Pai2str(j+2,0).c_str());

		}
		printf("\n");
	}
	printf("dora is:");
	for(i=0;i<bak->n_dora;++i)
		printf("%s ",tools->Pai2str(bak->dora[i],0).c_str());
	printf("\n");
	printf("actlist:\n");
	for(i=0;i<N_ACT;++i)
		if(actlist[i])printf("%d, %s\n",i,ACT_NAME[i]);
	tehai_14();
/*	int aflag=0;
	while(aflag==0){
		cout << "Input act:";
		if(!(cin >> act) || cin.peek()!='\n'){printf("Illegal act\n");cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');}
		if(act<0 || act>N_ACT){ printf("Illegal act\n");continue;}
		if(actlist[act]==0){printf("Wrong act\n");continue;}
		aflag=1;
	}
	
	if (act==ACT_TSUMOGIRI) {
		request(ACT_TSUMOGIRI);
		return;
	} else if (act==ACT_TEKIRI){ */
	cout << "select act:";
	char str[128],inp[128];
	//scanf("%s",str);
	cin.getline(str,128);
	sprintf(inp,"player_act %d %s",pos_me,str);
	input->one(inp);
	
//	if (bak->dacya == 0)	request(ACT_TSUMOGIRI);
//	else request(ACT_CANCEL);
}

void AI_Player::settings(int narg, char **arg)
{

}


void AI_Player::player_act(int narg, char** arg)
{
	int p=1,np;
	int i;
	
	if(narg==1){
		go_default();
		return;
	}
	if (!strcmp(arg[1],"pon") && actlist[ACT_PON]) {
		request(ACT_PON);
	} else if (!strcmp(arg[1],"kan") && actlist[ACT_KAN]) {
		if(bak->dacya!=0)request(ACT_KAN);
	} else if (!strcmp(arg[1],"ankan") && actlist[ACT_KAN_SELF] && narg>=3) {
		np = atoi(arg[2]);
		if(np<1 || np>tools->CountPai(bak->tehai)) go_default();
		else {
			cpai = th[np-1].idx;
			request(ACT_KAN_SELF);
		}
	} else if (!strcmp(arg[1],"chii") && actlist[ACT_CHII] && narg>=3) {
		np = atoi(arg[2]);
		if(np<1 || np>tools->CountPai(bak->tehai)) go_default();
		else {
			cpai = th[np-1].idx;
			aka = th[np-1].aka;
			request(ACT_CHII);
		}
	} else if (!strcmp(arg[1],"riichi") && actlist[ACT_RIICHI] && narg>=3) {
		np = atoi(arg[2]);
		if(np<1 || np>tools->CountPai(bak->tehai)) go_default();
		else {
			sutehai = th[np-1].idx;
			aka = th[np-1].aka;
			request(ACT_RIICHI);
		}
	} else if (!strcmp(arg[1],"tsumo") && actlist[ACT_AGARI_TSUMO]) {
		request(ACT_AGARI_TSUMO);
	} else if (!strcmp(arg[1],"ron") && actlist[ACT_AGARI_RON]) {
		request(ACT_AGARI_RON);
	} else {
		np = atoi(arg[1]);
		if(np<1 || np>tools->CountPai(bak->tehai)) go_default();
		else {
			sutehai = th[np-1].idx;
			aka = th[np-1].aka;
			request(ACT_TEKIRI);
		}
	}

}

void AI_Player::go_default()
{
	int nc;
	if (bak->dacya!=0) request(ACT_CANCEL);
	else {
		if(actlist[ACT_TSUMOGIRI])	request(ACT_TSUMOGIRI);
		else {
			nc=tools->CountPai(bak->tehai);
			sutehai = th[nc-1].idx;
			aka = th[nc-1].aka;
			request(ACT_TEKIRI);
		}
	}
}
