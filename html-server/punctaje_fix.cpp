#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <string.h>

#include "rules_fix.h"
#include <unistd.h>

using namespace std;

struct context {
	time_t theTime;
	int timeLeft;
	string command;
	void debug() const { cout<<"[CONTEXT]: tick "<<theTime<<" ("<<setw(4)<<timeLeft<<") entered '"<<command<<"'\n"; }
};

std::vector<context> contexts;

context handleLine(string line);
std::string handler(std::string hn, time_t theTime);

void runSimulation(std::vector<context> contexts);

std::string skewResults(std::string command);

int main() {

    // reset files
    {ofstream out("commands.txt");}
    {ofstream out("backup.txt");}

    // read config
    ifstream input("commands.backup");
	string line;
	while (getline(input, line)) {
		context theContext = handleLine(line);
		if (theContext.timeLeft == 0) {
			contexts.clear();
            contexts.emplace_back(move(theContext));
			continue;
		}
		contexts.emplace_back(move(theContext));

    }

    runSimulation(contexts);

	return 0;
}

void runSimulation(std::vector<context> contexts) {
    std::cout << "\nSTARTING SIMULATION\n-------------------\n\n";

    bool firstRun = false;
    int oldTimeTick = 0;
    for (const auto &context : contexts) {
        if (!firstRun) {

            oldTimeTick = context.theTime;

            readconfig();
            start_contest(context.theTime);

            firstRun = true;

            continue;
        }

        for (int i = oldTimeTick + 1; i < context.theTime; i++) {
            handler("data", i);
        }
        oldTimeTick = context.theTime;

        context.debug();
        handler(context.command, context.theTime);
    }
}

context handleLine(string line) {
	size_t location;
	vector<string> items;
	while ((location = line.find("   -   ")) != string::npos) {
		string item = line.substr(0, location);
		items.emplace_back(move(item));
		line = line.substr(location + 7);
	}
	items.emplace_back(move(line));
	time_t theTime = atol(items[0].c_str());
	int timeLeft = atol(items[1].c_str());
	string command = items[2];
	return {theTime, timeLeft, command};
}


std::string handler(std::string hn, time_t theTime) {
    hn = skewResults(hn);
	if (hn != "data") {
		std::ofstream fs;
	  	fs.open ("commands.txt", std::fstream::out | std::fstream::app);
        fs<<theTime<<"   -   "<<say_time_left(theTime)<<"   -   "<<hn<<"\n";
	  	fs.close();
  	}

	const char *text = hn.c_str();
	tick(theTime);
	if (strcmp(text,"ok")==0)
		return "ok";
	if (strcmp(text,"start")==0){
		d("Starting contest");
		start_contest(theTime);
		return "started";
	}
	if (text[0]=='S'){
		d("Changing Special Problem");
		char tmp[16384]={0};
		memcpy(tmp,text,sizeof(char) * strlen(text));
		strtok(tmp,"_");
		char *p=strtok(NULL,"_");
		int tta=atoi(p);
		p=strtok(NULL,"_");
		int prbl=atoi(p);
		set_team_bonus(tta,prbl,theTime);
		return "set team bonus";
	}
	if (strcmp(text,"OAIE")==0){
		d("ACCEPTING NEW CLIENTS");
		return "ACCEPTING NEW CLIENTS";
	}
	if (strcmp(text,"time")==0){
		d("Sending timeleft to console");
		return say_time_left(theTime);
	}
	if (strcmp(text,"data")==0){
		return say_drawables(theTime);
	}
	if (strstr(text,"add_")){
		char tmp[16384]={0};
		memcpy(tmp,text,sizeof(char) * strlen(text));
		strtok(tmp,"_");
		char *p=strtok(NULL,"_");
		int tta=atoi(p),add_amm=0;
		p=strtok(NULL,"_");
		add_amm=atoi(p);
		memset(tmp,0,sizeof(tmp));

		add_team_score(tta,add_amm);
		return "added";
	}
	if (strstr(text,"a_")){
		char tmp[16384]={0};
		memcpy(tmp,text,sizeof(char) * strlen(text));
		strtok(tmp,"_");
		int t,p,a;
        char *Q=strtok(NULL,"_");
        t=atoi(Q);
		Q=strtok(NULL,"_");
		p=atoi(Q);
		Q=strtok(NULL,"_");
		a=atoi(Q);
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"TEAM %d PROBLEM %d ANSWER %d",t,p,a);
		s(tmp);
		add_answer(t,p,a);
		return "added";
	}
	if (strcmp(text,"PULA")==0){
		return "NO LONGER ACCEPTING THREADS";

	}
	return "command not recognised";
}

std::string skewResults(std::string command) {
    using vct = std::vector<int>;
    static vector<vct> vector;
    // init
    if (vector.empty()) {
        ifstream in("punctaje_fix.txt");
        for (int i = 0; i < probleme; i++) {
            string items;
            getline(in, items);
            stringstream itemsSplitted(items);
            vct possibles;
            int possibility;
            while (itemsSplitted>>possibility) {
                possibles.emplace_back(possibility);
            }
            vector.emplace_back(possibles);
        }

        int index = 0;
        for (const auto & item : vector) {
            std::cout<<index++<<": ";
            for (const auto & itemul : item) {
                std::cout<<itemul<<" ";
            }
            std::cout<<"\n";
        }
    }

    string commandBefore = command;
    if (command[0] != 'a' || command[1] != '_') {
        return command;

    }



    stringstream items(command);
    string team;
    string problem;
    string answer;

    getline(items, team, '_');
    getline(items, team, '_');
    getline(items, problem, '_');
    getline(items, answer, '_');

    int teamS = atoi(team.c_str());
    int problemS = atoi(problem.c_str());
    int answerS = atoi(answer.c_str());

//    std::cout<<teamS << " " << problemS << " " << answerS << "\n";


    bool foundNew = false;
    for (const auto &possibility : vector[problemS - 1]) {
//        std::cout<<possibility<<" " << answerS<<"\n";
        if (answerS == possibility) {
            foundNew = true;
            break;
        }
    }
    if (foundNew)
        answerS = vector[problemS - 1][0];

    string newAnswer = std::to_string(answerS);
    while (newAnswer.length() < 4)
        newAnswer = "0" + newAnswer;

    command = "a_" + std::to_string(teamS) + "_" + std::to_string(problemS) + "_" + newAnswer + "_";
    if (command != commandBefore)
        std::cout<<"  **  SKEWING '"<<commandBefore<<"' => '"<<command<<"'\n";
//    exit(0);
    return command;
}
