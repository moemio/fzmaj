#ifdef AI_CLASS

AIStyle(player, AI_Player)

#else

#ifndef MAJ_AI_PLAYER_H
#define MAJ_AI_PLAYER_H

#include "ai.h"

namespace FZMAJ_NS {

class AI_Player : public AI {
public:
	AI_Player(class FZMAJ *);
	virtual ~AI_Player();
	void compute();
	void settings(int, char**);

};

}

#endif
#endif
