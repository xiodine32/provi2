//SERVER

#include "main.h"
#include "rules.h"
#include <iostream>

vector<client> clients;
vector<client> tempo;
stack<int> todestroy;

bool listening=true;
int server;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void *Threading(void *) {
	while (true) {
		while (listen(server, 3)) usleep(100);
		int c=accept(server,NULL,NULL);
		if (c != -1){
			if (listening){
				d("FOUND A CLIENT!");
                std::cout<<"CLIENT IS ON SOCKET "<<c<<"\n";
				client temp;
				temp.s=c;
				temp.destroyed=false;
				clients.push_back(temp);
			} else s("Rejected a connection");
		}
		usleep(100);
	}
}
#pragma clang diagnostic pop

void handle_text(int writeSocket,const char *text){
	tick();
	if (strcmp(text,"ok")==0)
		return;
	if (strcmp(text,"start")==0){
		d("Starting contest");
		start_contest();
		return;
	}
	if (text[0]=='S'){
		d("Changing Special Problem");
		char tmp[16384]={0};memcpy(tmp,text,sizeof(tmp));
		strtok(tmp,"|");
		char *p=strtok(NULL,"|");
		int tta=atoi(p);
		p=strtok(NULL,"|");
		int prbl=atoi(p);
		set_team_bonus(tta,prbl);
		return;
	}
	if (strcmp(text,"OAIE")==0){
		listening=true;
		d("ACCEPTING NEW CLIENTS");
	}
	if (strcmp(text,"time")==0){
		d("Sending timeleft to console");
		char toSend[204]={0};
		strcpy(toSend,say_time_left().c_str());
		write(writeSocket,toSend,sizeof(toSend));
		return;
	}
	if (strcmp(text,"data")==0){
		char toSend[16384]={0};
		strcpy(toSend,say_drawables().c_str());
		write(writeSocket,toSend,sizeof(toSend));
		return;
	}
	if (strstr(text,"add|")){
		char tmp[16384]={0};memcpy(tmp,text,sizeof(tmp));
		strtok(tmp,"|");
		char *p=strtok(NULL,"|");
		int tta=atoi(p),add_amm=0;
		p=strtok(NULL,"|");
		add_amm=atoi(p);
		memset(tmp,0,sizeof(tmp));

		add_team_score(tta,add_amm);
		return;
	}
	if (strstr(text,"a|")){
		char tmp[16384]={0};memcpy(tmp,text,sizeof(tmp));
		strtok(tmp,"|");
		int t,p,a;
        char *Q=strtok(NULL,"|");t=atoi(Q);
		Q=strtok(NULL,"|");p=atoi(Q);
		Q=strtok(NULL,"|");a=atoi(Q);
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"TEAM %d PROBLEM %d ANSWER %d",t,p,a);
		s(tmp);
		add_answer(t,p,a);
		return;
	}
	if (strcmp(text,"PULA")==0){
		listening=false;
		d("NO LONGER ACCEPTING THREADS");
		return;

	}
	s("command not recognised");
	s(text);
	return;
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfor-loop-analysis"
int main(){

	d("Reading configuration");
	FILE *fileBackup=fopen("backup.txt","r");
	if (fileBackup!=NULL){
		fclose(fileBackup);
		s("LOADING BACKUP DATA");
		load_backup();
	} else{
		s("Starting new SERVER");
		readconfig();
	}


	d("Loading winsock2");
	d("Loading server");


	server=socket(AF_INET,SOCK_STREAM,0);

	sockaddr_in anews;
	anews.sin_port = htons(1030);
	anews.sin_addr.s_addr = INADDR_ANY;
	anews.sin_family = AF_INET;

	bool running=true;

	if (bind(server, (sockaddr*)&anews, sizeof(anews)) < 0) {
		d("ERROR BINDING");
		goto cleanup;
	}

	d("Waiting for clients. DERP.");
	d("Starting Thread #1");

	pthread_t test;
	pthread_create(&test, NULL, Threading, NULL);
	d("Starting main loop");

	while (running){
		while (!todestroy.empty()){
			clients.erase(clients.begin()+todestroy.top());
			d("Deleted a client");
			todestroy.pop();
		}
		tempo=clients;
		int t=-1;
		for (vector<client>::iterator i=tempo.begin();i!=tempo.end();i++){
			++t;
			if (i->destroyed)continue;
			char text[16384] = {0};
			ssize_t out=recv(i->s, text, sizeof(text), MSG_WAITALL);

			if (out>0) {
                if (!strstr("dataok", text))
                    std::cout<<"Received some stuff: "<<out<<" with data: '"<<text<<"'\n";
				handle_text(i->s,text);
			}
			else{
				if (out==0 || out == -1){
					todestroy.push(t);
				}
			}
		}
		usleep(13);
	}
	cleanup:
	d("Finishing");

	d("All Done");
	getchar();
	return 0;
}
#pragma clang diagnostic pop
