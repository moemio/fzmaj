#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "sys/stat.h"
#include "unistd.h"
#include "input.h"
#include "tools.h"
#include "syanten.h"
#include "memory.h"
#include "error.h"
#include "style_command.h"
#include "run.h"
#include "agari.h"
#include "game.h"

#define DELTALINE 256
#define DELTA 4

using namespace FZMAJ_NS;

Input::Input(FZMAJ *maj, int argc, char **argv) : Pointers(maj)
{

	maxline = 0;
	maxcopy = 0;
	maxwork = 0;
	line = NULL;
	narg = maxarg = 0;
	arg = NULL;
	nfile = 1;

	command_map = new std::map<std::string,CommandCreator>();

#define COMMAND_CLASS
#define CommandStyle(key,Class) \
  (*command_map)[#key] = &command_creator<Class>;
#include "style_command.h"
#undef CommandStyle
#undef COMMAND_CLASS

}

/* ---------------------------------------------------------------------- */

Input::~Input()
{
  // don't free command and arg strings
  // they just point to other allocated memory

  memory->sfree(line);
  memory->sfree(arg);
  delete command_map;
}


/* ----------------------------------------------------------------------
   process all input from infile
   infile = stdin or file if command-line arg "-in" was used
------------------------------------------------------------------------- */

void Input::file()
{
  	int m,n;
  
  	while (1) {
    
    // read a line from input script
    // n = length of line including str terminator, 0 if end of file
    // if line ends in continuation char '&', concatenate next line
    
      	m = 0;
      	while (1) {
			maxline = 0;

        	if (maxline-m < 2) reallocate(line,maxline,0);
        	if (fgets(&line[m],maxline-m,infile) == NULL) {
          	if (m) n = strlen(line) + 1;
          	else n = 0;
          	break;
        }
        m = strlen(line);
        if (line[m-1] != '\n') continue;
        
        m--;
        while (m >= 0 && isspace(line[m])) m--;
        if (m < 0 || line[m] != '&') {
          	line[m+1] = '\0';
          	n = m+2;
          	break;}
      	}
    
    // if n = 0, end-of-file
    // error if label_active is set, since label wasn't encountered
    // if original input file, code is done
    // else go back to previous input file
    
    if (n == 0) {
        if (infile != stdin) {
          	fclose(infile);
          	infile = NULL;
        }
        nfile--;
      	if (nfile == 0) break;
     	continue;
    }
    
    if (n > maxline) reallocate(line,maxline,n);
    
    // echo the command unless scanning for label
    
    if (screen) fprintf(screen,"%s\n",line);
    if (logfile) fprintf(logfile,"%s\n",line);
    
    // parse the line
    // if no command, skip to next line in input script
    
    parse();
    if (command == NULL) continue;
    
    // execute the command
    
    if (execute_command()) {
      	char *str = new char[maxline+32];
      	sprintf(str,"Unknown command: %s",line);
     	error->all(FLERR,str);}
  	}
}

/* ----------------------------------------------------------------------
   process all input from filename
   called from library interface
------------------------------------------------------------------------- */

void Input::file(const char *filename)
{
  // error if another nested file still open, should not be possible
  // open new filename and set infile,  nfile
  // call to file() will close filename and decrement nfile

    if (nfile > 1)
      	error->all(FLERR,"Invalid use of library file() function");

    if (infile && infile != stdin) fclose(infile); 
    infile = fopen(filename,"r");
    if (infile == NULL) {
      	char str[128];
      	sprintf(str,"Cannot open input script %s",filename);
      	error->all(FLERR,str);
    }
    nfile = 1;
  
  	file();
}

/* ----------------------------------------------------------------------
   copy command in single to line, parse and execute it
   return command name to caller
------------------------------------------------------------------------- */

char *Input::one(const char *single)
{
  	int n = strlen(single) + 1;
  	if (n > maxline) reallocate(line,maxline,n);
  	strcpy(line,single);
  
  // echo the command unless scanning for label
  
    if (screen) fprintf(screen,"%s\n",line);
    if (logfile) fprintf(logfile,"%s\n",line);
  
  // parse the line
  // if no command, just return NULL
  
  	parse();
  	if (command == NULL) return NULL;
  
  // execute the command and return its name
  
  	if (execute_command()) {
    	char *str = new char[maxline+32];
    	sprintf(str,"Unknown command: %s",line);
    	error->all(FLERR,str);
  }
  
  return command;
}

/* ----------------------------------------------------------------------
   parse copy of command line by inserting string terminators
   strip comment = all chars from # on
   replace all $ via variable substitution
   command = first word
   narg = # of args
   arg[] = individual args
   treat text between single/double quotes as one arg
------------------------------------------------------------------------- */

void Input::parse()
{
  // duplicate line into copy string to break into words
  
  	int n = strlen(line) + 1;
  	if (n > maxcopy) reallocate(copy,maxcopy,n);
  	strcpy(copy,line);
  
  // strip any # comment by replacing it with 0
  // do not strip # inside single/double quotes
  
  	char quote = '\0';
  	char *ptr = copy;
  	while (*ptr) {
    	if (*ptr == '#' && !quote) {
      		*ptr = '\0';
      		break;
    	}
    	if (*ptr == quote) quote = '\0';
    	else if (*ptr == '"' || *ptr == '\'') quote = *ptr;
    	ptr++;
 	}
  
  // perform $ variable substitution (print changes)
  // except if searching for a label since earlier variable may not be defined
  
  
  // command = 1st arg in copy string
  
  	char *next;
  	command = nextword(copy,&next);
  	if (command == NULL) return;
  
  // point arg[] at each subsequent arg in copy string
  // nextword() inserts string terminators into copy string to delimit args
  // nextword() treats text between single/double quotes as one arg
  
  	narg = 0;
  	ptr = next;
  	while (ptr) {
    	if (narg == maxarg) {
      		maxarg += DELTA;
      		arg = (char **) memory->srealloc(arg,maxarg*sizeof(char *),"input:arg");
    	}
    arg[narg] = nextword(ptr,&next);
    if (!arg[narg]) break;
    narg++;
    ptr = next;
	}
}

/* ----------------------------------------------------------------------
   find next word in str
   insert 0 at end of word
   ignore leading whitespace
   treat text between single/double quotes as one arg
   matching quote must be followed by whitespace char if not end of string
   strip quotes from returned word
   return ptr to start of word
   return next = ptr after word or NULL if word ended with 0
   return NULL if no word in string
------------------------------------------------------------------------- */

char *Input::nextword(char *str, char **next)
{
  	char *start,*stop;
  
  	start = &str[strspn(str," \t\n\v\f\r")];
  	if (*start == '\0') return NULL;
  
  	if (*start == '"' || *start == '\'') {
    	stop = strchr(&start[1],*start);
    	if (!stop) error->all(FLERR,"Unbalanced quotes in input line");
    	if (stop[1] && !isspace(stop[1]))
      	error->all(FLERR,"Input line quote not followed by whitespace");
    	start++;
  	} else stop = &start[strcspn(start," \t\n\v\f\r")];
  
  	if (*stop == '\0') *next = NULL;
  	else *next = stop+1;
  	*stop = '\0';
  	return start;
}

void Input::reallocate(char *&str, int &max, int n)
{
  	if (n) {
    	while (n > max) max += DELTALINE;
  	} else max += DELTALINE;
  	str = (char *) memory->srealloc(str,max*sizeof(char),"input:str");
}

int Input::execute_command()
{
  	int flag = 1;
  	if (!strcmp(command,"run_test")) run_test();
	else if (!strcmp(command,"check")) check_pai();
	else if (!strcmp(command,"ai_style")) ai_style();
	else if (!strcmp(command,"agari_test")) check_agari();
  
  	else flag = 0;
  
  // return if command was listed above
  
  	if (flag) return 0;
  
  // invoke commands added via style_command.h
  
  	if (command_map->find(command) != command_map->end()) {
    	CommandCreator command_creator = (*command_map)[command];
    	command_creator(maj,narg,arg);
    return 0;
  }
  
  // unrecognized command
  
  return -1;
}


template <typename T>
void Input::command_creator(FZMAJ *maj, int narg, char **arg)
{
  T cmd(maj);
  cmd.command(narg,arg);
}

void Input::run_test()
{
	if (screen) fprintf(screen, "Hello, world!\n");
}

void Input::ai_style()
{
	if (narg < 2 ) error->all(FLERR, "Illegal ai_style command");
	int pos = atoi(arg[0]);
	game->create_ai(arg[1],pos);
}

void Input::check_agari()
{
	int c[34],last,n;
	last = tools->Str2pai(arg[0],c);
	n = tools->CountPai(c);
	if(n==14) n=agari->agari_test(c);
	else return;
	printf("n=%d\n",n);
}

void Input::check_pai()
{
	int i,n;
	int nst;
	int c[34], last;
	Bakyou *bak = new Bakyou;
	int iarg = 1, tharg;

	if (!game->started)
		game->start(1);
	game->createEmptyBakyou(bak, 0);

	if (narg==0) error->all(FLERR, "Illegal check command");

	last = tools->Str2pai(arg[0], c);
	n = tools->CountPai(c);
	if(n>14) error->all(FLERR, "Illegal pai number");
	for(i=0;i<34;++i) {
		bak->tehai[i] = c[i];
	}
	bak->syanpai = last;
	bak->act = ACT_AGARI_TSUMO;
	bak->dacya = 0;

	while (iarg < narg) {
		if (!strcmp(arg[iarg],"-chii") ||
			!strcmp(arg[iarg],"-ns")) {
			if (iarg+1 > narg) error->all(FLERR, "Invalid check command-line");
			tools->Str2pai(arg[iarg+1],bak->naki_syuntsu[0]);
			bak->n_naki_syuntsu[0] = tools->CountPai(bak->naki_syuntsu[0]);
			bak->n_naki[0] += bak->n_naki_syuntsu[0];
			iarg += 2;
		} else if (!strcmp(arg[iarg],"-pon") ||
				   !strcmp(arg[iarg],"-nk")) {
			if (iarg+1 > narg) error->all(FLERR, "Invalid check command-line");
			tools->Str2pai(arg[iarg+1],bak->naki_kotsu[0]);
			bak->n_naki_kotsu[0] = tools->CountPai(bak->naki_kotsu[0]);
			bak->n_naki[0] += bak->n_naki_kotsu[0];
			iarg += 2;
		} else if (!strcmp(arg[iarg],"-kan") ||
				   !strcmp(arg[iarg],"-nmk")) {
			if (iarg+1 > narg) error->all(FLERR, "Invalid check command-line");
			tools->Str2pai(arg[iarg+1],bak->naki_kan[0]);
			bak->n_naki_kan[0] = tools->CountPai(bak->naki_kan[0]);
			bak->n_naki[0] += bak->n_naki_kan[0];
			iarg += 2;
		} else if (!strcmp(arg[iarg],"-ankan") ||
				   !strcmp(arg[iarg],"-nak")) {
			if (iarg+1 > narg) error->all(FLERR, "Invalid check command-line");
			tools->Str2pai(arg[iarg+1],bak->naki_ankan[0]);
			bak->n_naki_ankan[0] = tools->CountPai(bak->naki_ankan[0]);
			iarg += 2;
		} else if (!strcmp(arg[iarg],"-riichi") ||
				   !strcmp(arg[iarg],"-r")) {
			bak->riichi[0]=1;
			iarg += 1;
		} else if (!strcmp(arg[iarg],"-ippatsu") ||
				   !strcmp(arg[iarg],"-ip")) {
			bak->jun[0]=1;
			iarg += 1;
		} else if (!strcmp(arg[iarg],"-ron")) {
			bak->dacya = 2;
			bak->act=ACT_AGARI_RON;
			iarg += 1;
		} else if (!strcmp(arg[iarg],"-last") ||
				   !strcmp(arg[iarg],"-l")) {
			bak->pai_ptr = bak->dead_ptr;
			iarg += 1;
		} else if (!strcmp(arg[iarg],"-rinsyan") ||
		           !strcmp(arg[iarg],"-rs")) {
			bak->act = ACT_KAN_SELF;
			iarg += 1;
		} else if (!strcmp(arg[iarg],"-tyankan") ||
				   !strcmp(arg[iarg],"-tyk")) {
			bak->act = ACT_KAN;
			bak->dacya = 2;
			iarg += 1;
		} 

	}
	printf ("  check  \n");
	printf ("\n\n*******************************\n");
	nst = syanten->calcSyantenAll(c);
	agari->checkAgari(bak);

	printf ("*******************************\n");
	delete bak;
}
