//CONSOLE CLIENT

#include "rules.h"

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <semaphore.h>

bool running=true;

int client;
char sendText[16384]={0},recvText[16384]={0};


sem_t semaphore_id;

// THE LOGIC

int handle_text(const char *text){
	if (strcmp(text,"help")==0) {
		printf("\nAdd an answer: a<TEAM><PROBLEM><ANSWER>\n");
		printf("Start Server: start\n");
		printf("Time Left: timeleft\n");
		printf("Set Team Special Problem: setp<TEAM><##> (with leading 0)\n");
		printf("Add/Subtract points: add_<TEAM>|<POINTS> (can be negative)\n");
		printf("\n");
		return 0;
	}
	if (strcmp(text,"START REC")==0){
		strcpy(sendText,"OAIE");
		return 1;
	}
	if (strcmp(text,"STOP REC")==0){
		strcpy(sendText,"PULA");
		return 1;
	}
	if (strcmp(text,"start")==0){
		printf("******************\n*STARTING CONTEST*\n******************\n");
		strcpy(sendText,"start");
		return 1;
	}
	if (strcmp(text,"timeleft")==0){
		strcpy(sendText,"time");
		printf("Requested time left\n");
		return 1;
	}
	if (strstr(text,"setp")){
		sprintf(sendText,"S|%c%c|%c%c",text[4],text[5],text[6],text[7]);
		printf("Set Special Problem for %c%c, problem #%c%c.\n",text[4],text[5],text[6],text[7]);
		return 1;
	}
	if (strstr(text,"add_")){
		text=(text+4);
		sprintf(sendText,"add|%s",text);
		printf("Did that, done that.\n");
		return 1;
	}
	if (text[0]=='a' && (text[1]>='0' && text[1]<='9')){
		char tmp[16384]={0};
		strcpy(tmp,text);
		printf("Answer for TEAM:%c%c - PROBLEM:%c%c - %c%c%c%c\n",tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7],tmp[8]);
		char ans[1024]={0};
		printf("Are you sure? (yes/YES/Y/y) :");scanf("%s%*c",ans);
		if (strcmp(ans,"yes")==0 || strcmp(ans,"YES")==0 || strcmp(ans,"Y")==0 || strcmp(ans,"y")==0){
			sprintf(sendText,"a|%c%c|%c%c|%c%c%c%c",tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7],tmp[8]);
			printf("[SENT]Answer for TEAM:%c%c, PROBLEM:%c%c with %c%c%c%c\n",tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7],tmp[8]);
			return 1;

		}
		printf("[NOT SENT]Answer for TEAM:%c%c - PROBLEM:%c%c - %c%c%c%c\n",tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7],tmp[8]);
		return 0;
	}
	printf("COMMAND NOT FOUND\n");	
	return 0;
}

void *listener(void *){
	while (running){
        memset(recvText, 0, sizeof(recvText));
		if (recv(client,recvText,sizeof(recvText), 0)>0){
            std::cout<<"RECEIVED TEXT '"<<recvText<<"'\n";
		}
		usleep(10);
	}
    return 0;
}

void *sender(void *){
    char tmp[16384]={"ok"};
    while (running){
        sem_wait(&semaphore_id);
        send(client,tmp,sizeof(tmp),0);
        sem_post(&semaphore_id);
        usleep(13);
    }
    return 0;
}
int main(){
    sem_init(&semaphore_id, 0, 1);

	char text[16384]={"127.0.0.1"};
	printf("ENTER SERVER IP:");scanf("%s%*c", text);
	d("start");
	d("host");
	struct sockaddr_in sin;

	memset( &sin, 0, sizeof sin );
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ((in_addr *)(gethostbyname(text)->h_addr))->s_addr;
	sin.sin_port = htons( 1030 );

	client=socket(AF_INET,SOCK_STREAM,0);

	

	if ( connect( client,(sockaddr*)&sin, sizeof sin ) < 0 )
	{
		d("ERROR CONNECTING");
		return 1;
	}
    pthread_t pListener;
    pthread_create(&pListener, NULL, listener, text);
    pthread_t pSender;
    pthread_create(&pSender, NULL, sender, text);

	while (running){
		printf("<SERVER>:");
        memset(text,0,sizeof(text));
        fgets(text, 16386, stdin);
        // terminated with newline, ARE YOU FUCKING KIDDING ME?!?
        text[strlen(text) - 1] = 0;
        memset(sendText,0,sizeof(sendText));
        if (handle_text(text)) {
            std::cout<<"sending '"<<sendText<<"'\n";
            sem_wait(&semaphore_id);
            if (send(client, sendText, sizeof(sendText), 0) <= 0) {
                d("ERR");
                running = false;
            };
            sem_post(&semaphore_id);
        }
	}
	d("end");
	getchar();
	return 0;
}