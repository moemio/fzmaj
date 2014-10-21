#ifdef AI_CLASS

AIStyle(general, AI_General)

#else

#ifndef MAJ_AI_TSUMOGIRI_H
#define MAJ_AI_TSUMOGIRI_H

namespace FZMAJ_NS {

class AI_General : public AI {
public:
	AI_General(class FZMAJ *);
	virtual ~AI_General();
	void kiru();

};

}

#endif
#endif
