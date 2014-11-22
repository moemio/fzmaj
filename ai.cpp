#include "ai.h"
#include "game.h"

using namespace FZMAJ_NS;

AI::AI(FZMAJ *maj) : Pointers(maj)
{}

AI::~AI(){}

void AI::request(int act)
{
	game->request(pos_me, act);
}
