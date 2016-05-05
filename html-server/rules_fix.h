#ifndef RULES
#define RULES

#include <string>
#include <sstream>

const int PENALITATE_GRESEALA=10;
const int BONUS_MAX=20;
const int PROBLEMA_INITIAL=20;
const int PUNCTAJ_ECHIPA_INITIAL=200;
const bool debug=true;


void d(const char *data);
void s(const char *data);
void readconfig();
void start_contest(time_t theTime);
void force_end_contest();
std::string say_time_left(time_t theTime);
std::string say_drawables(time_t theTime);
void add_team_score(int team,int points);
void add_answer(int team,int problem,int answer);
void set_team_name(int team, char name[8]);
void set_team_bonus(int team,int pb, time_t theTime);
void load_backup(time_t theTime);
struct team {
	char nume[40];
	int punctaj;
	int corecte;
	int problema_bonus;
	int raspunsuri[101];
	bool corect_pb;
} ;
struct problema{
	int raspuns;
	int puncte;
	int bonus;
	bool answered[101];

};

void tick(time_t theTime);
#endif