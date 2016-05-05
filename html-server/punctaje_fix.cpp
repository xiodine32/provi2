#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "rules_fix.h"

using namespace std;

struct context {
	time_t theTime;
	int timeLeft;
	string command;
	void debug() const { cout<<"[CONTEXT]: tick "<<theTime<<" ("<<setw(4)<<timeLeft<<") entered '"<<command<<"'\n"; }
};

std::vector<context> contexts;

context handleLine(string line);

int main() {

	ifstream input("commands.txt");
	string line;
	while (getline(input, line)) {
		context theContext = handleLine(line);
		if (theContext.timeLeft == 0) {
			contexts.clear();
			continue;
		}
		contexts.emplace_back(move(theContext));
		
	}

	for (const auto &item : contexts) {
		item.debug();
	}

	return 0;
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

	if (hn != "data") {
		std::ofstream fs;
	  	fs.open ("commands.txt", std::fstream::out | std::fstream::app);
	  	fs<<time(NULL)<<"   -   "<<say_time_left(theTime)<<"   -   "<<hn<<"\n";
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