#include "server.h"

#include "rules.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void load() {
	FILE *fileBackup=fopen("backup.txt","r");
	if (fileBackup!=NULL){
		fclose(fileBackup);
		s("LOADING BACKUP DATA");
		load_backup();
	} else{
		s("Starting new SERVER");
		readconfig();
	}
}

std::string handler(std::string hn) {

	if (hn != "data") {
		std::ofstream fs;
	  	fs.open ("commands.txt", std::fstream::out | std::fstream::app);
	  	fs<<time(NULL)<<"   -   "<<say_time_left()<<"   -   "<<hn<<"\n";
	  	fs.close();
  	}

	const char *text = hn.c_str();
	tick();
	if (strcmp(text,"ok")==0)
		return "ok";
	if (strcmp(text,"start")==0){
		d("Starting contest");
		start_contest();
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
		set_team_bonus(tta,prbl);
		return "set team bonus";
	}
	if (strcmp(text,"OAIE")==0){
		d("ACCEPTING NEW CLIENTS");
		return "ACCEPTING NEW CLIENTS";
	}
	if (strcmp(text,"time")==0){
		d("Sending timeleft to console");
		return say_time_left();
	}
	if (strcmp(text,"data")==0){
		return say_drawables();
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

int main() {

	load();

	JsonServer server;

	server.run(handler);

	return 0;
}
