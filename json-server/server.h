#ifndef __SERVER_H
#define __SERVER_H

#include <string>

typedef std::string (*JSHandler_f)(std::string request);

class JsonServer {
	int socketHandler;
	bool running;
	void init();
	std::string computeMimeType(bool hasFile, std::string textPath);
public:
	JsonServer();
	~JsonServer();
	void run(const JSHandler_f &handler);
};

#endif
