#ifndef MAJ_SYANTEN_H
#define MAJ_SYANTEN_H

#include "pointers.h"

namespace FZMAJ_NS {

class Syanten : protected Pointers {

public:
	Syanten(class FZMAJ *);
	~Syanten();

	void init();
	int calcSyantenAll(int tehai[]);
	int calcSyantenNo713(int tehai[]);

	int c[34];
	int n_mentsu;
	int n_tatsu;
	int n_toitsu;
	int min_syanten;
	int st_normal;
	int st_7;
	int st_13;

protected:
	
	int n_eval;
	int f_n4;
	int f_koritsu;
	int n_jidahai;

	void updateResult();
	void scan713();
	void scanNormal(int init_mentsu);
	void Run(int depth);
	void removeJihai(int nc);
	void i_anko(int k);
	void d_anko(int k);
	void i_toitsu(int k);
	void d_toitsu(int k);
	void i_syuntsu(int k);
	void d_syuntsu(int k);
	void i_tatsu_r(int k);
	void d_tatsu_r(int k);
	void i_tatsu_k(int k);
	void d_tatsu_k(int k);
	void i_koritsu(int k);
	void d_koritsu(int k);
	int count34();
	int calcSyanten(bool isSkip713);
};

}

#endif
