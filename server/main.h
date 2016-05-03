#ifndef MAINH
#define MAINH


#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

#include <stdio.h>
#include <vector>
#include <stack>
#include <sstream>

using namespace std;
struct client{
	int s;
	bool destroyed;
}; 
void handle_text(const char *text);

#endif