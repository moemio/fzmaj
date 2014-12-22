#ifndef MAJ_YAKU_H
#define MAJ_YAKU_H

#include "pointers.h"

namespace FZMAJ_NS {

const char YAKU_NAME[46][25] = {
	{"Menchin Tsumo   "},
	{"Riichi          "},
	{"Chan Kan        "},
	{"Rinshan Kaihou  "},
	{"Haitei Raoyue   "},
	{"Houtei Raoyuu   "},
	{"Yakuhai Bafuu   "},
	{"Yakuhai Jifuu   "},
	{"Yakuhai Sangen  "},
	{"Tan Yao         "},
	{"I-pei kou       "},
	{"Pinfu           "},
	{"Chanta          "},
	{"Ikki Tsukan     "},
	{"SanShoku Doujun "},
	{"Double Riichi   "},
	{"Sanshoku Doukou "},
	{"San Kantsu      "},
	{"Toitoi          "},
	{"San Ankou       "},
	{"Shou Sangen     "},
	{"Hon Rou Tou     "},
	{"Chii Toitsu     "},
	{"Jonchan         "},
	{"Hon Itsu        "},
	{"Ryanpei kou     "},
	{"Chin Itsu       "},
	{"Ippatsu         "},
	{"Dora            "},
	{"Ura Dora        "},
	{"Aka Dora        "},
	{"Ten Hou         "},
	{"Chi Hou         "},
	{"Dai Sangen      "},
	{"Su Ankou        "},
	{"Su Ankou Tanki  "},
	{"Tsu Ii Sou      "},
	{"Ryu Ii Sou      "},
	{"Chin Rou Tou    "},
	{"Kokushi Musou   "},
	{"Kokushi 13 Machi"},
	{"Dai Su Shi      "},
	{"Shou Su Shi     "},
	{"Su Kantsu       "},
	{"Chuuren Boutou  "},
	{"Jon Chuuren     "}
};

enum YAKU_SHURUI {
	YAKU_MENTSUMO=0,					// 0
	YAKU_RIICHI,						// 1
	YAKU_TYANKAN,						// 2
	YAKU_RINSYAN,						// 3
	YAKU_HAITEI,						// 4
	YAKU_HOUTEI,	 	 				// 5
	YAKU_YAKUHAI_BAFUU,					// 6
	YAKU_YAKUHAI_JIFUU,					// 7
	YAKU_YAKUHAI_SANGEN,				// 8
	YAKU_TANYAO,						// 9
	YAKU_IPPEI,							// 10
	YAKU_PINFU,							// 11
	YAKU_HONCHAN, 						// 12
	YAKU_IKKITSUKAN, 					// 13
	YAKU_SANSHOKU,						// 14
	YAKU_WRIICHI,						// 15
	YAKU_TONKO, 						// 16
	YAKU_SANKANTSU,						// 17
	YAKU_TOITOI,						// 18
	YAKU_SANANKO,						// 19
	YAKU_SYAOSANGEN,					// 20
	YAKU_HONROU,						// 21
	YAKU_CHIITOI,						// 22
	YAKU_JONCHAN, 						// 23
	YAKU_HONIISOU,						// 24
	YAKU_RYANPEI,						// 25
	YAKU_CHINIISOU,						// 26
	YAKU_IPPATSU,						// 27	
	YAKU_DORA,							// 28
	YAKU_URA,							// 29
	YAKU_AKA,							// 30
	YAKU_TENHOU,						// 31
	YAKU_CHIHOU,						// 32
	YAKU_DASANGEN,						// 33
	YAKU_SUANKO,						// 34
	YAKU_SUANKO_TANKI,					// 35
	YAKU_TSUIISOU, 						// 36
	YAKU_RYUIISOU, 						// 37
	YAKU_CHINROU,						// 38
	YAKU_KOKUSHI,						// 39
	YAKU_KOKUSHI_13,					// 40
	YAKU_DASUSHI, 						// 41
	YAKU_SYAOSUSHI, 					// 42
	YAKU_SUKANTSU,						// 43
	YAKU_CHUUREN, 						// 44
	YAKU_JONCHUUREN						// 45
};

class Yaku : protected Pointers  {
public:	
	int countYaku(Bakyou *, PATTERN &);
	Yaku(class FZMAJ *);
	~Yaku();
protected:
	
	int fan;
	int fu;
	int yakus[46];
	int score;
	int fan_nodora;


	int IsMenTsumo();
	int IsRiichi();
	int IsTyankan();
	int IsRinsyan();
	int IsHaitei();
	int IsHoutei();
	int IsYakuhaiBafuu();
	int IsYakuhaiJifuu();
	int IsYakuhaiSangen();
	int IsTanyao();
	int CheckPeiko();
	int IsIppei();
	int IsPinfu();

	int IsChantai();
	int IsIkki();	
	int IsSanshoku();
	int IsWRiichi();
	int IsTonko();
	int IsSankantsu();
	int IsToitoi();
	int IsSananko();
	int IsSyaosangen();
	int IsHonrou();
	int IsChiitoi();
	
	int IsJonchan();
	int IsHoniisou();
	int IsRyanpei();
	int IsChiniisou();

	int IsIppatsu();
	int IsDora();
	int IsUra();
	int IsAka();

	int IsTenhou();
	int IsChihou();
	int IsDasangen();
	int IsSuanko();
	int IsSuanko_tanki();
	int IsTsuiisou();
	int IsRyuiisou();
	int IsChinrou();
	int IsKokushi();
	int IsKokushi_13();
	int IsDasushi();
	int IsSyaosushi();
	int IsSukantsu();
	int IsChuuren();

	void countFan();
	int countFu();
	int countScore();

	PATTERN *pattern;
	Bakyou *bak;

	int yakufan[46];
	int Is19(int k);
	int Is19z(int k);

};

}

#endif
