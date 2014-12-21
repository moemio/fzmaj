#include "stdio.h"
#include "ai.h"
#include "ai_player.h"
#include "tools.h"
#include "input.h"
#include <iostream>

using namespace FZMAJ_NS;
using namespace std;

AI_Player::AI_Player(FZMAJ *maj) : AI(maj)

{
	th.clear();
	th.resize(14);
}

AI_Player::~AI_Player() {
}

void AI_Player::compute()
{
	int i,j,p,akap;
	printf("ai %d computing, dacya %d\n",pos_me,bak->dacya);
	printf("my tehai:\n");
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

void AI_Player::tehai_14()
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
					th[p].aka = 1;
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

void AI_Player::player_act(int narg, char** arg)
{
	int p=1,np;
	int i;
	for(i=0;i<narg;++i)
		printf("arg %d is %s\n",i,arg[i]);
	/*
	while(p<=narg){
		if(!strcmp(arg[p],"pon")){
			if(bak->dacya!=0) {
				request(ACT_PON);
				break;
			} else { request(ACT_TSUMOGIRI); break;}
		++p;
		} else if (!strcmp(arg[p],"ron")) {
			if(bak->dacya!=0) {
				request(ACT_AGARI_RON);
			} else {
				request(ACT_TSUMOGIRI);
				break;}
		} else {
			if(bak->dacya!=0){ request(ACT_CANCEL);break;}
			else {
				np = atoi(arg[p]);
				if (np<1 || np>=tools->CountPai(bak->tehai)) {
					printf("np = %d, nc = %d\n",np,tools->CountPai(bak->tehai));
					request(ACT_TSUMOGIRI);
					break;
				}
				else{
					sutehai=th[np-1].idx;
					printf("sutehai is  %d, %s\n",sutehai,tools->Pai2str(sutehai,0).c_str());
					aka = th[np-1].aka;
					request(ACT_TEKIRI);
					break;
				}
			}
		}
	} */
	
	if (!strcmp(arg[1],"pon") && actlist[ACT_PON]) {
		request(ACT_PON);
	} else if (!strcmp(arg[1],"kan") && actlist[ACT_KAN]) {
		request(ACT_KAN);
	} else if (!strcmp(arg[1],"ankan") && actlist[ACT_KAN_SELF]) {
		np = atoi(arg[2]);
		if(np<1 || np>tools->CountPai(bak->tehai)) go_default();
		else {
			cpai = th[np-1].idx;
			request(ACT_KAN_SELF);
		}
	} else if (!strcmp(arg[1],"chii") && actlist[ACT_CHII]) {
		np = atoi(arg[2]);
		if(np<1 || np>tools->CountPai(bak->tehai)) go_default();
		else {
			cpai = th[np-1].idx;
			aka = th[np-1].aka;
			request(ACT_CHII);
		}
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
	if (bak->dacya!=0) request(ACT_CANCEL);
	else request(ACT_TSUMOGIRI);
}
