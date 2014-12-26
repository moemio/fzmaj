#ifdef AI_CLASS

AIStyle(player, AI_Player)

#else

#ifndef MAJ_AI_PLAYER_H
#define MAJ_AI_PLAYER_H

#include "ai.h"
#include <string>
#include <vector>

namespace FZMAJ_NS {

class AI_Player : public AI {
public:
	AI_Player(class FZMAJ *);
	virtual ~AI_Player();
	void player_act(int narg, char** arg);
	void compute();
	void settings(int, char**);
/*
	struct Pai{
		int idx;
		int aka;
		string name;
	};

	void tehai_14();
	vector<Pai>th;
*/
	void go_default();
};

}

#endif
#endif
