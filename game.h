#ifndef MAJ_GAME_H
#define MAJ_GAME_H

#include "pointers.h"
#include <vector>
#include <string>
#include <map>
#include "string.h"

//#define GAME_DEBUG

using namespace std;

namespace FZMAJ_NS {

class Game : protected Pointers {
public:


	Game(class FZMAJ *);
	~Game();
	void start(long);
	void createEmptyBakyou(struct Bakyou *, int);
	void updateBakyou(class Bakyou *, int);
	int started;

	class AI *ai[4];
	char *ai_style;
	typedef AI *(*AICreator)(FZMAJ *);
	void create_ai(const char*, int);
	std::map<std::string,AICreator> *ai_map;
	
	class AI *new_ai(const char *);
	void request(int,int);

	int is_test;


protected:
	
	int pai[136];

	int bafuu;
	int jifuu[4];
	int kyoku;
	int hajioya;
	int oya;
	int honba;
	int residue;
	int n_dora;
	int pai_ptr;
	int jun[4];
	int dead_ptr;
	int pos_ptr;
	int riichi[4];
	int furiten[4];
	int n_naki[4];
	int naki_kotsu[4][34];
	int naki_syuntsu[4][34];
	int naki_kan[4][34];
	int naki_ankan[4][34];
	int n_naki_kotsu[4];
	int n_naki_syuntsu[4];
	int n_naki_kan[4];
	int n_naki_ankan[4];

	int aka_river[4][3];
	int aka_tehai[4][3];
	int aka_dorahyouji[3];
	int aka_naki[4][3];

	int tsumo_hai;
	int sutehai;
	int cur_act;
	int cur_pos;
	int cur_aka;
	int dacya;

	int agari_flag;
	int naki_flag;
	int ren_zoku;
	int n_kan_tot;

	vector< vector<int> > river;
	vector< vector<int> > river_stat;
	vector<int> dora;
	vector<int> ura;
	
	int endgame;
	int ura_check;

	int score[4];
	int Ryukyoku;
	int tehai[4][34];
	int juni[4];
	int tenpai[4][34];
	string agarilist[4];
	int in_agari_list(int,int);
	void check_furiten(int);
	int queue[11];
	int **actlist;

	long seed;


	void clearGame();
	void initGame();
	void initPai(int []);
	void initBakyou(class Bakyou *, int);
	int gameLoop();
	
	int checkRequest(int);
	void update_juni();
	void clear_actlist(int);

	void tsumogiri(int);
	void tekiri(int);
	void tsumoru(int);
	void kan_tsumoru(int);
	void pon(int);
	void chii(int,int,int);
	void kan(int);
	int chiiable(int,int);
	int ponable(int,int);
	int kanable(int,int);
	int riichiable(int);
	int riichiable_sute(int);
	void agari_tsumo(int);
	void agari_ron(int);
	void riichi_sengen(int);
	int is_19z(int);
	int is_kyukyu(int);
	int is_nagashi_mankan(int);

	void add_queue(int,int);
	void deal_queue();
	void get_ura(int);
	int c_dora(int);
	void add_dora();
	void ryukyoku(int);
	void make_actlist(int);

	void request_ai(int);

	template <typename T> static AI *ai_creator(FZMAJ *);

};

}

#endif
