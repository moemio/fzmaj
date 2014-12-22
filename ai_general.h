#ifdef AI_CLASS

AIStyle(general, AI_General)

#else

#ifndef MAJ_AI_GENERAL_H
#define MAJ_AI_GENERAL_H

#include "ai.h"

namespace FZMAJ_NS {

class AI_General : public AI {
public:
	AI_General(class FZMAJ *);
	virtual ~AI_General();
	void compute();
	void settings(int, char**);
};

}

#endif
#endif
