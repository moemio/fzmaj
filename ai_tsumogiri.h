#ifdef AI_CLASS

AIStyle(tsumogiri, AI_Tsumogiri)

#else

#ifndef MAJ_AI_TSUMOGIRI_H
#define MAJ_AI_TSUMOGIRI_H

#include "ai.h"

namespace FZMAJ_NS {

class AI_Tsumogiri : public AI {
public:
	AI_Tsumogiri(class FZMAJ *);
	virtual ~AI_Tsumogiri();
	void compute();
	void settings(int, char**);

};

}

#endif
#endif
