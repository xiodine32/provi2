#include "server.h"

#include <iostream>
#include <fstream>
#include <sstream>


#define d(data) std::cout<<"[DEBUG]: "<<data<<"\n"


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void JsonServer::init() {
	d("json server init");
	socketHandler = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	int f = 1;
	if (setsockopt(socketHandler, SOL_SOCKET, SO_REUSEADDR, &f, sizeof(int)) < 0) {
		perror("setsockopt");
		exit(1);
	}
	if (socketHandler < 0) {
		perror("open");
		exit(1);
	}

	sockaddr_in socketData = {0};

	socketData.sin_family = AF_INET;
	socketData.sin_addr.s_addr = htonl(INADDR_ANY);
	socketData.sin_port = htons(1030);

	int status = bind(socketHandler, (sockaddr *) &socketData, sizeof(sockaddr_in));
	if (status < 0) {
		perror("bind");
		exit(1);
	}

	listen(socketHandler, SOMAXCONN);
}

JsonServer::JsonServer() {
	d("json server construct");

	init();

}
JsonServer::~JsonServer() {
	d("json server del");
}

std::string JsonServer::computeMimeType(bool hasFile, std::string textPath) {
	if (!hasFile)
		return "text/plain";
	std::string extension = textPath.substr(textPath.rfind('.') + 1);
	if (extension == "js")
		return "application/javascript";
	if (extension == "svg")
		return "image/svg+xml";
	if (extension == "html")
		return "text/html";
	if (extension == "jpg" || extension == "jpeg")
		return "image/jpeg";
	if (extension == "png")
		return "image/png";
	if (extension == "css")
		return "text/css";
	d(extension);
	return "";
}
void JsonServer::run(const JSHandler_f &handler) {
	d("json server run");
	running = true;

	while (running) {
		int socketConnection;
		socketConnection = accept(socketHandler, NULL, NULL);
		if (socketConnection < 0) {
			perror("accept");
			continue;
		}

		char text[8192] = {0};
		int result = recv(socketConnection, text, sizeof(text), 0);
		if (result <= 0) {
			perror("recv empty");
            close(socketConnection);
            continue;
		}

		// trim to first new line
		*(strchr(text, '\n') - 1) = 0;

		// remove " HTTP/1.1" and "GET /"
		std::string textString = text;
		textString = textString.substr(5, textString.size() - 9 - 5);

		std::string textPath = "res/" + textString;
		if (textPath == "res/")
			textPath += "index.html";

		std::stringstream outData;

		bool hasFile = false;
		FILE *f = fopen(textPath.c_str(), "r");
		if (f != NULL) {
			fclose(f);
			hasFile = true;
		}


		if (textString.find("favicon.ico") != std::string::npos) {
			outData<<"HTTP/1.0 404 Not Found\n";
		} else {
			outData<<"HTTP/1.1 200 OK\n";
			outData<<"cache-control:no-cache, no-store, max-age=0, must-revalidate\n";
			outData<<"connection: close\n";
			outData<<"content-type:"<<computeMimeType(hasFile, textPath)<<"; charset=utf-8\n";
			outData<<"pragma:no-cache\n\n";


			if (hasFile) {
				std::ifstream input(textPath.c_str());
				outData << input.rdbuf();
			} else {
				outData<<handler(textString);
			}
		}
		std::string out = outData.str();
		send(socketConnection, out.c_str(), out.size(), 0);
		close(socketConnection);
	}
}
