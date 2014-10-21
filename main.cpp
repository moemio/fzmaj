#include "fzmaj.h"
#include "input.h"
#include "string.h"

using namespace FZMAJ_NS;

int main(int argc, char **argv)
{
	FZMAJ *fzmaj = new FZMAJ(argc, argv);
	fzmaj->input->file();
	delete fzmaj;
}
